#include <iostream>
#include <memory>
#include <algorithm>
#include <vector>
#include <EASTL/vector_map.h>
#include <EASTL/scoped_ptr.h>




//////////////////////////////////////////////////////////////////////////////////////////
/// \brief implementation imposed by EASTL
/////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////


//Need to implement the following 'new' operators for libEASTL.a to work
//file:///tmp/EASTL/doc/SampleNewAndDelete.cpp
//file:///tmp/EASTL/doc/EASTL%20FAQ.html#Info.15
//////////////////////////////////////

#include <EABase/eabase.h>
#include <stddef.h>
#include <new>

#include <stdio.h>
#include <stdlib.h>

#include <map>
#include <memory>
#include <chrono>


///////////////////////////////////////////////////////////////////////////////
// throw specification wrappers, which allow for portability.
///////////////////////////////////////////////////////////////////////////////

#if defined(EA_COMPILER_NO_EXCEPTIONS) && (!defined(__MWERKS__) || defined(_MSL_NO_THROW_SPECS)) && !defined(EA_COMPILER_RVCT)
    #define THROW_SPEC_0    // Throw 0 arguments
    #define THROW_SPEC_1(x) // Throw 1 argument
#else
    #define THROW_SPEC_0    throw()
    #define THROW_SPEC_1(x) throw(x)
#endif


///////////////////////////////////////////////////////////////////////////////
// operator new used by EASTL
///////////////////////////////////////////////////////////////////////////////

void* operator new[](size_t size, const char* /*name*/, int /*flags*/,
                     unsigned /*debugFlags*/, const char* /*file*/, int /*line*/) THROW_SPEC_1(std::bad_alloc)
{
    return malloc(size);
}

void* operator new[](size_t size, size_t alignment, size_t alignmentOffset, const char* /*name*/,
                     int flags, unsigned /*debugFlags*/, const char* /*file*/, int /*line*/) THROW_SPEC_1(std::bad_alloc)
{
    // Substitute your aligned malloc.
    return /*malloc_aligned*/aligned_alloc(size, alignment/*, alignmentOffset*/);
}



///////////////////////////////////////////////////////////////////////////////
// Other operator new as typically required by applications.
///////////////////////////////////////////////////////////////////////////////

void* operator new(size_t size) THROW_SPEC_1(std::bad_alloc)
{
    return malloc(size);
}


void* operator new[](size_t size) THROW_SPEC_1(std::bad_alloc)
{
    return malloc(size);
}


///////////////////////////////////////////////////////////////////////////////
// Operator delete, which is shared between operator new implementations.
///////////////////////////////////////////////////////////////////////////////

void operator delete(void* p) THROW_SPEC_0
{
    if(p) // The standard specifies that 'delete NULL' is a valid operation.
        free(p);
}


void operator delete[](void* p) THROW_SPEC_0
{
    if(p)
        free(p);
}

//////////////////////////////////////////////////////////////////////////////////////////
/// \brief implementation imposed by EASTL
/////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////




//EA vector map lookup OVERPASSES traditional STL map lookup, as it's way more cache friendly
//and under the hood is an "random access iterator" array/vector and not a bunch of nodes

//Note iterators are invalidated if we start removing elements from it

typedef eastl::vector_map<std::string, char> VMAP;

typedef std::map<std::string, char> STLMAP;


void vecMap_lookUp_EA()
{

    //EA vector map lookup
    eastl::scoped_ptr<VMAP> v_map(new VMAP);

    //fill vector map
    for(int i = 0; i < 10; i++)
    {
        v_map->insert(eastl::pair<std::string,char>("a%d"+i,'a'+i));
    }

    //print out vector map elements (key value)
    for(const auto &i: *v_map)
    {
        std::cout<<i.first<<"-->"<<i.second<<std::endl;
    }

    //VECTOR MAP look up for the:  key=8
    VMAP::iterator  ea_it;
    ea_it = v_map->find("0");
    if (ea_it != v_map->end())
    {
        std::cout<<(*ea_it).first<<"-->"<<(*ea_it).second<<std::endl;
    }
}


void vecMap_lookUp_STL()
{
    /////////////////////////////////////////////////////

    //traditional STL map lookup
    //EA vector map lookup
    std::shared_ptr<STLMAP> stl_map(new STLMAP);

    //fill STL map
    for(int i = 0; i < 10; i++)
    {
        stl_map->insert(std::pair<std::string,char>("a%d"+i,'a'+i) );
    }

    //print out STL map elements (key value)
    for(const auto &i: *stl_map)
    {
        std::cout<<i.first<<"-->"<<i.second<<std::endl;
    }

    //STL MAP look up for the:  key=8
    STLMAP::iterator  stlIter;
    stlIter = stl_map->find("0");
    if (stlIter != stl_map->end())
    {
        std::cout<<(*stlIter).first<<"-->"<<(*stlIter).second<<std::endl;
    }

}


int main()
{
    auto begin = std::chrono::high_resolution_clock::now();
    auto end = std::chrono::high_resolution_clock::now();

    double averageEA=0;
    int counterEA=0;
    double sumTimeEA=0;


//    for(int i=0;i<100;i++)
    {
        auto begin = std::chrono::high_resolution_clock::now();
        vecMap_lookUp_EA();
        counterEA++;
        auto end = std::chrono::high_resolution_clock::now();
        sumTimeEA+=std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count();
    }
    averageEA=sumTimeEA/counterEA;





    //initialize variables
    double averageSTL=0;
    int counterSTL=0;
    double sumTimeSTL=0;

//    for(int i=0;i<100;i++)
    {
        auto begin = std::chrono::high_resolution_clock::now();
        vecMap_lookUp_STL();
        counterSTL++;
        auto end = std::chrono::high_resolution_clock::now();
        sumTimeSTL+=std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count();
    }
    averageSTL=sumTimeSTL/counterSTL;




    std::cout << "average time EAMAP with counterEA " << counterEA << " iterations: "<< averageEA << "ns" << std::endl;
    std::cout << "average time STLMAP with counterSTL " << counterSTL << " iterations: "<< averageSTL << "ns" << std::endl;


//    end = std::chrono::high_resolution_clock::now();
//    std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count() << "ns" << std::endl;





    return 0;
}

