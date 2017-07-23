#pragma once

#include <memory>

template <class T>
class Singleton
{
  public:
    typedef std::shared_ptr<T> TypePtr;

  public:
    static TypePtr&
    instance()
    {
      if (instance_ == nullptr)
        instance_ = std::shared_ptr<T>(new T);
      
      return instance_;
    }

  public:
    Singleton(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton& operator=(Singleton&&) = delete;

  protected:
    Singleton() = default;

  private:
    static TypePtr instance_;
};

template <class T>
typename Singleton<T>::TypePtr Singleton<T>::instance_ = nullptr;
