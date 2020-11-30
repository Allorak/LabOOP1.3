#ifndef QUEUE_H
#define QUEUE_H

#include <memory>
#include <stdexcept>

template<typename T> class Queue {
public:
    Queue() = default;
    Queue(const Queue& other);
    Queue(Queue&& other) = default;
    ~Queue() = default;
    Queue& operator=(const Queue& other) = default;
    Queue& operator=(Queue&& other) = default;
    T& Head();
    void Dequeue();
    void Enqueue(const T& value);
    void Enqueue(T&& value);
    size_t Count() const;
    bool Empty() const;
    void Erase();
    friend bool operator== (const Queue &q1, const Queue &q2)
    {
        if(q1._size!=q2._size)return false;
        std::shared_ptr<QueueElement> iterator1 = q1._front;
        std::shared_ptr<QueueElement> iterator2 = q2._front;
        while( iterator1!=nullptr && iterator2 != nullptr && iterator1->GetValue()==iterator2->GetValue())
        {
            iterator1=iterator1->GetNext();
            iterator2=iterator2->GetNext();
        }
        return (iterator1==nullptr && iterator2==nullptr);
    }
    friend bool operator!=(const Queue &q1, const Queue &q2)
    {return !(q1==q2);};
private:
    class QueueElement {
    public:
        QueueElement() = delete;
        QueueElement(const T& value, std::shared_ptr<QueueElement>&& next) :
            _value{value}, _next{std::move(next)} { }
        QueueElement(T&& value, std::shared_ptr<QueueElement>&& next) :
            _value{std::move(value)}, _next{std::move(next)} { }
        T& value() { return _value; }
        std::shared_ptr<QueueElement>& next() { return _next; }
        void SetNext(std::shared_ptr<QueueElement> next) {_next = next;}
    private:
        T _value;
        std::shared_ptr<QueueElement> _next{nullptr};
    };
    std::shared_ptr<QueueElement> _front{nullptr};
    std::shared_ptr<QueueElement> _back{nullptr};
    size_t _size{0};
};

template<typename T> Queue<T>::Queue(const Queue& other)
{
    std::shared_ptr<QueueElement> iterator = other._front;
    while(iterator)
    {
        Enqueue(iterator->value());
        iterator = iterator->next();
    }
}

template<typename T> T& Queue<T>::Head() {
    if (_front == nullptr) {
        throw std::out_of_range("front() of empty queue");
    }
    return _front->value();
}

template<typename T> void Queue<T>::Dequeue()
{
    if(_size==0)
    {
        throw std::out_of_range("Empty queue");
    }
    _front = _front->next();
    _size--;
}

template<typename T> void Queue<T>::Enqueue(const T& value_)
{
    if (_back == nullptr)
    {
        _front=_back=std::make_shared<QueueElement>(std::move(value_),nullptr);
    }
    else
    {
        std::shared_ptr<QueueElement> object = std::make_shared<QueueElement>(value_,nullptr);
        _back->SetNext(object);
        _back=object;
    }
    _size++;
}

template<typename T> void Queue<T>::Enqueue(T&& value) {
    if(_back == nullptr)
    {
        _front=_back=std::make_shared<QueueElement>(std::move(value),nullptr);
    }
    else
    {
        std::shared_ptr<QueueElement> object = std::make_shared<QueueElement>(std::move(value),nullptr);
        _back->SetNext(object);
        _back=object;
    }
    _size++;
}

template<typename T> size_t Queue<T>::Count() const {
    return _size;
}

template<typename T> bool Queue<T>::Empty() const {
    return _front == nullptr;
}

template<typename T> void Queue<T>::Erase() {
    _front = nullptr;
    _back = nullptr;
    _size = 0;
}

#endif // QUEUE_H
