#include "CLContext.h"
#include <utility>

using namespace std;

CLContext::CLContext() : context(nullptr)
{
}

CLContext::CLContext(CLContext&& other)
{
    *this = move(other);
}

CLContext::CLContext(const CLContext& other) : context(nullptr)
{
    *this = other;
}

CLContext::~CLContext()
{
    if (this->context)
    {
        clReleaseContext(this->context);
    }
}

CLContext& CLContext::operator=(CLContext&& other)
{
    if (this->context)
    {
        clReleaseContext(this->context);
    }

    this->context = other.context;
    other.context = nullptr;
    return *this;
}

CLContext& CLContext::operator=(const CLContext& other)
{
    if (other.context && CL_SUCCESS != clRetainContext(other.context))
    {
        throw runtime_error("Failed to retain context");
    }

    if (this->context)
    {
        clReleaseContext(this->context);
    }
    
    this->context = other.context;
    return *this;
}

bool CLContext::Create(cl_device_id device)
{
    if (this->context)
    {
        clReleaseContext(this->context);
    }
    
    cl_int error;
    this->context = clCreateContext(nullptr, 1, &device, nullptr, nullptr, &error);

    if (CL_SUCCESS != error)
    {
        this->context = nullptr;
        return false;
    }

    return true;
}

CLBuffer CLContext::CreateBuffer(uint64_t flags, size_t bytes)
{
    return CLBuffer::Create(this->context, flags, bytes);
}