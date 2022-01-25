#pragma once

#include "CLEvent.h"
#include <cstdint>
#include <initializer_list>

class CLBuffer
{
private:
    CLBuffer(cl_mem);
public:
    CLBuffer(CLBuffer&&);
    CLBuffer(const CLBuffer&);
   ~CLBuffer();

    CLBuffer& operator=(CLBuffer&&);
    CLBuffer& operator=(const CLBuffer&);

    CLEvent& Event() const
    {
        return this->event;
    }
    operator CLEvent&() const
    {
        return this->event;
    }

    bool Map(cl_command_queue);
    bool Map(cl_command_queue, const std::initializer_list<CLEvent>& waitList);
    void Unmap(const std::initializer_list<CLEvent>& waitList = {});

    size_t Length() const;
    
    void* Mapped() const
    {
        return this->map;
    }

    operator cl_mem() const
    {
        return this->mem;
    }
    
    operator bool() const
    {
        return !!this->mem;
    }

    static CLBuffer Create(cl_context, uint64_t flags, size_t bytes);

    static uint64_t RO;
    static uint64_t WO;
    static uint64_t RW;

private:
    cl_mem mem;
    void*  map;
    cl_command_queue que;

    mutable CLEvent event;
};