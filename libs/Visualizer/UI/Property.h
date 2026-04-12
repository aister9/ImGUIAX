#pragma once

#include <functional>
#include <utility>
#include <vector>

namespace ImGUIAX
{
    namespace Visualizer
    {
        class PropertyBase
        {
        public:
            virtual ~PropertyBase() = default;
        };

        template <typename T>
        class Property : public PropertyBase
        {
        public:
            using ChangeListener = std::function<void(const T& oldValue, const T& newValue)>;
            using InvalidationListener = std::function<void()>;

            class Connection
            {
            public:
                Connection() = default;
                Connection(Property* owner, std::size_t id) : owner_(owner), id_(id) {}

                Connection(Connection&& other) noexcept
                {
                    owner_ = other.owner_;
                    id_ = other.id_;
                    other.owner_ = nullptr;
                    other.id_ = 0;
                }

                Connection& operator=(Connection&& other) noexcept
                {
                    if (this != &other)
                    {
                        disconnect();
                        owner_ = other.owner_;
                        id_ = other.id_;
                        other.owner_ = nullptr;
                        other.id_ = 0;
                    }
                    return *this;
                }

                Connection(const Connection&) = delete;
                Connection& operator=(const Connection&) = delete;

                void disconnect()
                {
                    if (owner_)
                    {
                        owner_->removeListener(id_);
                        owner_ = nullptr;
                        id_ = 0;
                    }
                }

                bool connected() const { return owner_ != nullptr; }

            private:
                Property* owner_ = nullptr;
                std::size_t id_ = 0;
            };

            Property() = default;
            explicit Property(T value) : value_(std::move(value)) {}

            const T& get() const { return value_; }

            void set(const T& value)
            {
                if (boundTo_)
                {
                    return;
                }
                setInternal(value, false);
            }

            bool isBound() const { return boundTo_ != nullptr; }

            void bind(Property<T>& other)
            {
                if (&other == this)
                {
                    return;
                }
                unbind();
                unbindBidirectional();

                boundTo_ = &other;
                boundListenerId_ = other.addListenerInternal(
                    [this](const T&, const T& newValue) { setFromBinding(newValue); },
                    nullptr);
                setFromBinding(other.get());
            }

            void unbind()
            {
                if (!boundTo_)
                {
                    return;
                }
                boundTo_->removeListener(boundListenerId_);
                boundTo_ = nullptr;
                boundListenerId_ = 0;
            }

            void bindBidirectional(Property<T>& other)
            {
                if (&other == this)
                {
                    return;
                }
                unbind();
                other.unbind();
                unbindBidirectional();
                other.unbindBidirectional();

                bidirectional_ = &other;
                other.bidirectional_ = this;
                setInternal(other.get(), true);
            }

            void unbindBidirectional()
            {
                if (!bidirectional_)
                {
                    return;
                }
                Property<T>* other = bidirectional_;
                bidirectional_ = nullptr;
                if (other->bidirectional_ == this)
                {
                    other->bidirectional_ = nullptr;
                }
            }

            void unbindBidirectional(Property<T>& other)
            {
                if (bidirectional_ == &other)
                {
                    unbindBidirectional();
                }
            }

            Connection addChangeListener(ChangeListener listener)
            {
                const std::size_t id = addListenerInternal(std::move(listener), nullptr);
                return Connection(this, id);
            }

            Connection addInvalidationListener(InvalidationListener listener)
            {
                const std::size_t id = addListenerInternal(nullptr, std::move(listener));
                return Connection(this, id);
            }

        private:
            struct Listener
            {
                std::size_t id;
                ChangeListener change;
                InvalidationListener invalidation;
            };

            void setInternal(const T& value, bool fromBinding)
            {
                if (value_ == value)
                {
                    return;
                }

                const T oldValue = value_;
                value_ = value;
                notifyInvalidation();
                notifyChange(oldValue, value_);

                if (!fromBinding && bidirectional_ && !updating_)
                {
                    updating_ = true;
                    bidirectional_->setFromBinding(value_);
                    updating_ = false;
                }
            }

            void setFromBinding(const T& value)
            {
                setInternal(value, true);
            }

            void notifyChange(const T& oldValue, const T& newValue)
            {
                for (const auto& listener : listeners_)
                {
                    if (listener.change)
                    {
                        listener.change(oldValue, newValue);
                    }
                }
            }

            void notifyInvalidation()
            {
                for (const auto& listener : listeners_)
                {
                    if (listener.invalidation)
                    {
                        listener.invalidation();
                    }
                }
            }

            std::size_t addListenerInternal(ChangeListener change, InvalidationListener invalidation)
            {
                const std::size_t id = ++nextId_;
                listeners_.push_back(Listener{ id, std::move(change), std::move(invalidation) });
                return id;
            }

            void removeListener(std::size_t id)
            {
                for (std::size_t i = 0; i < listeners_.size(); ++i)
                {
                    if (listeners_[i].id == id)
                    {
                        listeners_.erase(listeners_.begin() + static_cast<long long>(i));
                        return;
                    }
                }
            }

            T value_{};
            std::size_t nextId_ = 0;
            std::vector<Listener> listeners_;

            Property<T>* boundTo_ = nullptr;
            std::size_t boundListenerId_ = 0;
            Property<T>* bidirectional_ = nullptr;
            bool updating_ = false;
        };
    }
}
