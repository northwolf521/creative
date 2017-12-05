#ifndef __ABSTRACT_MODULE_H__
#define __ABSTRACT_MODULE_H__

#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <iostream>
#include <string>
#include <map>
#include "tcp_conn_base.h"
#include "log.h"

template <typename _Tin, typename _Tout>
class function_convert {
public:
    function_convert(const _Tin& in_param) : _in_param(in_param) {

    }

public:
    _Tout operator() () {
        union _types {
            _Tin  _local_in;
            _Tout _local_out;
        };

        _types conv;
        conv._local_in = _in_param;
        return conv._local_out;
    }

private:
    _Tin _in_param;
};

template <typename _Tx>
inline void* convert_function(_Tx fun) {
    return function_convert<_Tx, void*>(fun)();
}

typedef struct _module_enter {
    uint32_t    msg_id;   //net msg id
    void*       func;     //dispatch func
    void*       maker;    //construct func
    void*       destruct; //destruct func
} module_enter;

#ifdef BEGIN_GAME_ENTRY
#undef BEGIN_GAME_ENTRY
#endif

#define BEGIN_GAME_ENTRY(CLASSNAME)                                         \
    public:                                                                 \
        typedef CLASSNAME ___calss_name__;                                  \
        static ___calss_name__*  __module_object__;                         \
    public:                                                                 \
        static module_enter* __get___module___enter___list___() {           \
            static module_enter  __enter__[] = {


#ifdef IMPL_GAME_ENTRY
#undef IMPL_GAME_ENTRY
#endif

#define IMPL_GAME_ENTRY(msg_id, func)                                       \
                {msg_id, convert_function(func), convert_function(&___calss_name__::___maker___), convert_function(&___calss_name__::___destructor___)},


#ifdef END_GAME_ENTRY
#undef END_GAME_ENTRY
#endif

#define END_GAME_ENTRY()                                                    \
                {0, 0, 0}                                                   \
            };                                                              \
            return __enter__;                                               \
        }                                                                   \
        static int ___maker___(void** pObj) {                               \
            if (!pObj) {                                                    \
                ERR_LOG(0, 0l, 0, "input param error, pObj is NULL");       \
                return -1;                                                  \
            }                                                               \
            if (__module_object__) {                                        \
                *pObj = (void*)___calss_name__::__module_object__;          \
                return 0;                                                   \
            }                                                               \
            try {                                                           \
                ___calss_name__::__module_object__ = new ___calss_name__(); \
                if (!___calss_name__::__module_object__) {                  \
                    ERR_LOG(0, 0l, 0, "input param error, pObj is NULL");   \
                    return -1;                                              \
                }                                                           \
                *pObj = ___calss_name__::__module_object__;                 \
                return 0;                                                   \
            } catch (...) {                                                 \
                ERR_LOG(0, 0l, 0, "input param error, pObj is NULL");       \
                return -1;                                                  \
            }                                                               \
            return -1;                                                      \
        }                                                                   \
        static void ___destructor___() {                                    \
            if (___calss_name__::__module_object__) {                       \
                delete ___calss_name__::__module_object__;                  \
                ___calss_name__::__module_object__ = NULL;                  \
            }                                                               \
        }

#define REGISTER_MODULE(CLASSNAME)                                          \
    CLASSNAME* CLASSNAME::__module_object__ = NULL;                         \
    int ___##CLASSNAME##__register_ret___ = register_module(CLASSNAME::__get___module___enter___list___());


int register_module(module_enter* enter);
int unregister_module();
int get_register_dispach(uint32_t msg_id, void** pObj, void** func);

#ifdef static_assert_v
#undef static_assert_v
#endif

#if defined(DEBUG) || defined(_DEBUG)
#if defined (_WIN32) || defined(WIN32)
#define static_assert_v(exp) do { typedef int static_assert_type[(exp)?1:0];} while(false)
#else
#define static_assert_v(exp) do { typedef int static_assert_type[(exp)?1:-1];} while(false)
#endif
#else
#define  static_assert_v(exp)
#endif

#ifdef STATIC_ASSERT
#undef STATIC_ASSERT
#endif
#define STATIC_ASSERT static_assert_v

#ifdef __case_call
#undef __case_call
#endif

#if defined(WIN32) || defined(_WIN32)
#define __case_call  __thiscall
#else
#define __case_call
#endif

#ifdef CASE_GET_ENTRY
#undef CASE_GET_ENTRY
#endif
#define CASE_GET_ENTRY(key)   CASE_GET_ENTRY_EX(__init_label__, key)

#ifdef CASE_GET_ENTRY_EX
#undef CASE_GET_ENTRY_EX
#endif
#define CASE_GET_ENTRY_EX(label, key)  __get_my_case_driver_table__##label##__(key)

#ifdef CALL_DISPATCH
#undef CALL_DISPATCH
#endif
#define CALL_DISPATCH(_key, dispath_fun_type) CALL_DISPATCH_EX(__init_label__, _key, dispath_fun_type)

#ifdef CALL_DISPATCH_EX
#undef CALL_DISPATCH_EX
#endif
#define CALL_DISPATCH_EX(label,_key, dispath_fun_type)  function_convert<void*, dispath_fun_type> (CASE_GET_ENTRY_EX(label, _key))()

#ifdef BEGIN_CASE_MAP
#undef BEGIN_CASE_MAP
#endif
#define BEGIN_CASE_MAP() BEGIN_CASE_EX_MAP(__init_label__)

#ifdef BEGIN_CASE_EX_MAP
#undef BEGIN_CASE_EX_MAP
#endif
#define BEGIN_CASE_EX_MAP(label)                                                \
    template <typename key_type>                                                \
    static void* __get_my_case_driver_table__##label##__(const key_type& key)   \
    {                                                                           \
        static struct table_driver_t                                            \
        {                                                                       \
            key_type _key;                                                      \
            void* _entry;                                                       \
        }_tables[] = {


#ifdef IMPL_CASE_MAP
#undef IMPL_CASE_MAP
#endif
#define IMPL_CASE_MAP(key, value)                                               \
        {key, convert_function(&value)},


#ifdef END_CASE_MAP
#undef END_CASE_MAP
#endif
#define END_CASE_MAP(err_func)                                                  \
            {key_type(), convert_function(&err_func)}                           \
        };                                                                      \
        unsigned int  suffix = 0;                                               \
        for (; suffix<sizeof(_tables)/sizeof(_tables[0])-1; ++suffix)           \
        {                                                                       \
            if (_tables[suffix]._key==key && _tables[suffix]._entry)            \
            {                                                                   \
                return _tables[suffix]._entry;                                  \
            }                                                                   \
        }                                                                       \
        return _tables[suffix]._entry;                                          \
    }


//example:
#if 0
class MyTest
{
public:
    BEGIN_CASE_EX_MAP(buy)
    IMPL_CASE_MAP("1", MyTest::test1)
    IMPL_CASE_MAP("2", MyTest::test2)
    IMPL_CASE_MAP("3", MyTest::test3)
    END_CASE_MAP(MyTest::errors)

public:
    MyTest()
        : m_xd(50)
    {}

public:
    int test(std::string key)
    {
#if 1
        return CALL_DISPATCH_EX(buy, key, int(__case_call*)(void*))(this);
#else
        // or
        return (this->*CALL_DISPATCH(key, int(MyTest::*)()))();
#endif
    }

    int test1()
    {
        printf("this is test1, m_xd: %d\n", m_xd);
        return 0;
    }


    int test2()
    {
        printf("this is test2, m_xd: %d\n", m_xd * 2);
        return 0;
    }

    int test3()
    {
        printf("this is test3, m_xd: %d\n", m_xd * 3);
        return 0;
    }

    int errors()
    {
        printf("this is error\n");
        return 0;
    }

private:
    int m_xd;
};


struct TestA
{
    int test(int x, int y, int z)
    {
        return 0;
    }

    int  dispatch()
    {
        this->test(1, 2, 3);
        return 0;
    }
};

int main()
{

    TestA  aa;
    aa.dispatch();

    MyTest tet;
    tet.test("1");
    tet.test("2");
    tet.test("3");
    tet.test("4");
    tet.test("5");


    getchar();
    return 0;
}
#endif

#endif
