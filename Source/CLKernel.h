#pragma once

#include <initializer_list>
#include <exception>
#include <vector>
#include <CL/cl.h>

class CLKernel
{
public:
    CLKernel(cl_kernel);
    CLKernel(CLKernel&&);
    CLKernel(const CLKernel&) = delete;
   ~CLKernel();

    CLKernel& operator=(CLKernel&&);
    CLKernel& operator=(const CLKernel&) = delete;

    void Size(const std::initializer_list<size_t>& global, const std::initializer_list<size_t>& local = {});

    const size_t* Global() const
    {
        return this->global.empty() ? nullptr : this->global.data();
    }
    
    const size_t* Local() const
    {
        return this->local.empty() ? nullptr : this->local.data();
    }
    
    cl_uint Dims() const
    {
        return (cl_uint)this->global.size();
    }

    template<typename T0, typename... Tx>
    bool Args(const T0& arg0, const Tx&... args)
    {
        return this->SetArgs(0, arg0, args...);
    }

    operator cl_kernel() const
    {
        return this->kernel;
    }

    operator bool() const
    {
        return !!this->kernel;
    }
private:
    template<typename T0>
    bool SetArgs(cl_uint index, const T0& arg0)
    {
        return CL_SUCCESS == clSetKernelArg(this->kernel, index, sizeof(arg0), &arg0);
    }

    template<typename T0, typename... Tx>
    bool SetArgs(cl_uint index, const T0& arg0, const Tx... args)
    {
        if (CL_SUCCESS != clSetKernelArg(this->kernel, index, sizeof(arg0), &arg0))
        {
            return false;
        }

        return this->SetArgs(index + 1, args...);
    }

private:
    cl_kernel kernel;
    std::vector<size_t> global;
    std::vector<size_t> local;
};