/*
 * attrtest.cpp
 *
 *  Created on: Sep 14, 2008
 *      Author: jasonr
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <math.h>
#include <time.h>
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE AttrTest
#include <boost/test/unit_test.hpp>
#include <boost/utility/empty_deleter.hpp>
#include <boost/shared_ptr.hpp>
#include <log/log.h>
#include <ut/make_unique.h>
#include <ut/exempt_ptr.h>

#include "core/exception.h"
#include "core/global.h"
#include "core/node.h"
#include "core/standardAttributes.h"
#include "core/typedAttribute.h"
#include "core/ostreamhelpers.h"

const float TOLERANCE = 0.0001;

namespace Sg
{

using IntPtr = std::exempt_ptr<int>;

template <typename... Args>
using PtrAttribute        = TypedAttribute<IntPtr, Args...>;

template <typename... Args>
using PtrAttributePtr     = std::exempt_ptr<TypedAttribute<IntPtr, Args...>>;

template <typename... Args>
using PtrFunctor          = Functor<IntPtr, Args...>;

template <typename... Args>
using PtrFunctorPtr       = typename FunctorPtr<IntPtr, Args...>::Type;

template <typename... Args>
using PtrLambdaFunctor    = LambdaFunctor<IntPtr, Args...>;

template <typename... Args>
using PtrLambdaFunctorPtr = typename LambdaFunctorPtr<IntPtr, Args...>::Type;


template <typename... Args>
class TypedAttribute<IntPtr, Args...> : public TypedAttributeBase<IntPtr, TypedAttribute<IntPtr, Args...>, Args...>
{
  private:
   using Super = TypedAttributeBase<IntPtr, TypedAttribute<IntPtr, Args...>, Args...>;

  public:
    TypedAttribute<IntPtr, Args...>();
    TypedAttribute<IntPtr, Args...>(const IntPtr &value);
    TypedAttribute<IntPtr, Args...>(IntPtr &&value);
    virtual ~TypedAttribute<IntPtr, Args...>() {};

    virtual std::string                 toString() const;
};


template <typename... Args>
TypedAttribute<IntPtr, Args...>::TypedAttribute() :
    Super("IntPtr", IntPtr())
{
}

template <typename... Args>
TypedAttribute<IntPtr, Args...>::TypedAttribute(const IntPtr &value) :
    Super("IntPtr", value)
{
}

template <typename... Args>
TypedAttribute<IntPtr, Args...>::TypedAttribute(IntPtr &&value) :
    Super("IntPtr", std::move(value))
{
}

template <typename... Args>
std::string
TypedAttribute<IntPtr, Args...>::toString() const
{
    std::stringstream out;
    out << Super::m_value;
    return out.str();
}

template <typename... Args>
bool
isPtrAttribute(const AttributePtr &attr)
{
    return isTypedAttribute<IntPtr, Args...>(attr);
}

template <typename... Args>
PtrAttributePtr<Args...>
asPtrAttribute(const AttributePtr &attr)
{
    return asTypedAttribute<IntPtr, Args...>(attr);
};

template <typename... Args>
std::unique_ptr<TypedAttribute<IntPtr, Args...>>
asPtrAttribute(std::unique_ptr<Attribute> attr)
{
    return asTypedAttribute<IntPtr, Args...>(std::move(attr));
};

} // namespace Sg

namespace Local
{

template<class SharedPointer>
struct Holder
{
    SharedPointer p;

    Holder(const SharedPointer &p) : p(p) {}
    Holder(const Holder &other) : p(other.p) {}
    Holder(Holder &&other) : p(std::move(other.p)) {}

    void operator () (...) const {}
};


template<class T>
boost::shared_ptr<T>
to_boost_ptr(const std::shared_ptr<T> &p)
{
    typedef Holder<boost::shared_ptr<T>> H;
    if(H * h = std::get_deleter<H, T>(p))
    {
        return h->p;
    }
    else
    {
        return boost::shared_ptr<T>(p.get(), Holder<std::shared_ptr<T>>(p));
    }
}

} // namespace Local

class Group : public Sg::Node
{
public:
    Group() = default;
    ~Group() = default;

    virtual std::string     nodeType() const { return "group"; }
    void initializeNode() {}
};

std::unique_ptr<Sg::Node>
createGroup(void)
{
    return std::make_unique<Group>();
}

class Attribute
{
  public:
      float memberFunction0();
      float memberFunction1(float t);
};

float
Attribute::memberFunction0()
{
    return M_PI;
}

float
Attribute::memberFunction1(float t)
{
    return t;
}

float
freeFunction0()
{
    return M_PI;
}

float
freeFunction1(const float &t)
{
    return t;
}

struct GlobalInit
{
    GlobalInit()
    {
        Sg::initialize();

        Sg::attributeFactory().registerCreator("testptr",  []() { return Sg::newAttribute<Sg::IntPtr>(); });

//        Log::addOutput(Local::to_boost_ptr(std::shared_ptr<std::ostream>(&std::cout, boost::empty_deleter())));
//        Log::addOutput(Local::to_boost_ptr(std::shared_ptr<std::ostream>(new std::ofstream("/tmp/ov.log"))));
//        Log::setVerbosity(2);
//        Log::setIndentSize(2);
//        Log::registerChannel("logging", "ov log");
//        Log::enable("sg:core");
//        Log::enable("sg:node");
    }
};

BOOST_GLOBAL_FIXTURE( GlobalInit );

BOOST_AUTO_TEST_CASE ( staticTest )
{
    Sg::FloatAttribute<> fa;
    BOOST_CHECK_CLOSE (fa.value(), 0.0, TOLERANCE);
    BOOST_CHECK_EQUAL (fa.hasKeys(), false);
    BOOST_CHECK_EQUAL (fa.hasDriver(), false);
}

BOOST_AUTO_TEST_CASE ( setValueTest )
{
    Sg::FloatAttribute<> fa;
    fa.set (M_PI_2);
    BOOST_CHECK_CLOSE(fa.value(), M_PI_2, TOLERANCE);
}

BOOST_AUTO_TEST_CASE( functorTest )
{
    Sg::FloatAttribute<>                  fa;
    Sg::TypedAttribute<float, float>    fa2;

    {
    // driven from a free function with no args
    Sg::FloatFunctorPtr<> widthFunctor(new Sg::FloatFunctor<>(freeFunction0));
    fa.setDriver(widthFunctor);
    BOOST_TEST_MESSAGE("Testing Functor Attribute from a free function");
    BOOST_CHECK_CLOSE (fa.value(), M_PI, TOLERANCE);
    BOOST_CHECK_EQUAL (fa.hasKeys(), false);
    BOOST_CHECK_EQUAL (fa.hasDriver(), true);
    }

    {
    // driven from a free function with a float arg
    std::exempt_ptr<Sg::Functor<float, float>> widthFunctor(new Sg::Functor<float, float>(freeFunction1));
    fa2.setDriver(widthFunctor);
    BOOST_TEST_MESSAGE("Testing Functor Attribute from a free function");
    BOOST_CHECK_CLOSE (fa2.value(static_cast<float>(M_PI/2.0)), M_PI/2.0, TOLERANCE);
    BOOST_CHECK_EQUAL (fa2.hasKeys(), false);
    BOOST_CHECK_EQUAL (fa2.hasDriver(), true);
    }

    {
    // driven from a member function with no args
    Attribute testAttr;
    Sg::FloatFunctorPtr<> widthFunctor(new Sg::FloatFunctor<>(&Attribute::memberFunction0, &testAttr));
    fa.setDriver(widthFunctor);
    BOOST_TEST_MESSAGE("Testing Functor Attribute from a member function");
    BOOST_CHECK_CLOSE (fa.value(), M_PI, TOLERANCE);
    BOOST_CHECK_EQUAL (fa.hasKeys(), false);
    BOOST_CHECK_EQUAL (fa.hasDriver(), true);
    }
}

BOOST_AUTO_TEST_CASE( drivenAttrTest )
{
    Sg::FloatAttribute<> fa1(M_PI);
    Sg::FloatAttribute<> fa2;
    Sg::FloatAttributePtr<> fa1Ptr(new Sg::FloatAttribute<>(fa1));

    {
    // driven from anotherteN attr
    fa2.setDriver(fa1Ptr);
    BOOST_TEST_MESSAGE("Testing Driven Attribute");
    BOOST_CHECK_CLOSE (fa2.value(), M_PI, TOLERANCE);
    BOOST_CHECK_EQUAL (fa2.hasKeys(), false);
    BOOST_CHECK_EQUAL (fa2.hasDriver(), true);
    }
}

BOOST_AUTO_TEST_CASE ( polymorphismTest )
{
    Sg::FloatAttribute<> fa;
    Sg::Attribute &a = dynamic_cast <Sg::Attribute &>(fa);
    BOOST_CHECK_EQUAL (a.hasKeys(), false);
}

BOOST_AUTO_TEST_CASE ( assignmentTest )
{
    // static asn
    Sg::FloatAttribute<> fa1(M_PI), fa2(M_PI_2);
    fa2 = fa1;
    BOOST_CHECK_CLOSE (fa1.value(), M_PI, TOLERANCE);
    BOOST_CHECK_CLOSE (fa2.value(), M_PI, TOLERANCE);
}

BOOST_AUTO_TEST_CASE ( polymorphicAssignmentTest )
{
    Sg::FloatAttribute<> fa1(M_PI), fa2(M_PI_2);
    Sg::IntAttribute<> ia;

    Sg::Attribute &a1 = dynamic_cast <Sg::Attribute &>(ia);
    Sg::Attribute &a2 = dynamic_cast <Sg::Attribute &>(fa1);
    BOOST_CHECK_THROW (a1 = a2, Sg::TypeExc);

    Sg::Attribute &a3 = dynamic_cast <Sg::Attribute &>(fa1);
    Sg::Attribute &a4 = dynamic_cast <Sg::Attribute &>(fa2);
    BOOST_CHECK_NO_THROW (a3 = a4);

    BOOST_CHECK_NO_THROW (a4 = a3);
    BOOST_CHECK_CLOSE (fa1.value(), M_PI_2, TOLERANCE);
    BOOST_CHECK_CLOSE (fa2.value(), M_PI_2, TOLERANCE);
}

BOOST_AUTO_TEST_CASE ( copyTest )
{
    Sg::FloatAttribute<> fa1, fa2;

    fa1.set (M_PI_2);
    fa2 = fa1;
    BOOST_CHECK_CLOSE (fa1.value(), fa2.value(), TOLERANCE);

    {
    // driven from a free function with no args
    Sg::IntAttribute<> widthAttr;
    Sg::FloatFunctorPtr<> functorPtr (new Sg::Functor<float> (freeFunction0));
    fa1.setDriver(functorPtr);
    BOOST_TEST_MESSAGE("Testing Functor Attribute from a free function");
    fa2 = fa1;
    BOOST_CHECK_CLOSE (fa1.value(), fa2.value(), TOLERANCE);
    }

    {
    // driven from a member function with no args
    Attribute attrTest;
    Sg::FloatFunctorPtr<> functorPtr (new Sg::Functor<float> (&Attribute::memberFunction0, &attrTest));
    fa1.setDriver(functorPtr);
    BOOST_TEST_MESSAGE("Testing Functor Attribute from a member function");
    fa2 = fa1;
    BOOST_CHECK_CLOSE (fa1.value(), fa2.value(), TOLERANCE);
    }
}

BOOST_AUTO_TEST_CASE ( objectAttrTest )
{
    Sg::registerCreator("group", createGroup);

    auto grpa = Sg::createNode("/grpa", "group");

    // create float attr with value 0.0
    auto fattr = Sg::asFloatAttribute(Sg::createAttribute("float"));
    fattr->set(0.0f);
    grpa->addAttribute("translation", std::move(fattr));

    // check that same name can't be added twice
    BOOST_CHECK_THROW (grpa->addAttribute("translation", Sg::createAttribute("float")), Sg::KeyExc);

    // check that it's 0.0
    BOOST_CHECK_CLOSE (Sg::asFloatAttribute(grpa->attributeByName("translation"))->value(), 0.0, TOLERANCE);

    // change it to 1.0 with a Sg::FloatAttribute
    grpa->setAttribute("translation", Sg::FloatAttribute<>(1.0));

    // check that it's 1.0
    BOOST_CHECK_CLOSE (Sg::asFloatAttribute(grpa->attributeByName("translation"))->value(), 1.0, TOLERANCE);

    // change it to 2.0
    grpa->setAttribute("translation", static_cast<float>(2.0));

    // check that it's 2.0
    BOOST_CHECK_CLOSE (Sg::asFloatAttribute(grpa->attributeByName("translation"))->value(), 2.0, TOLERANCE);

    // try to set it to a string
    BOOST_CHECK_THROW (grpa->setAttribute("translation", std::string("foo")), Sg::TypeExc);

    // add an int attr
    grpa->addAttribute("size", Sg::createAttribute("int"));

    // test attributesByName
    BOOST_CHECK (grpa->attributesByType<int>()[0] == grpa->attributeByName("size"));
    BOOST_CHECK (grpa->attributesByType<float>()[0] == grpa->attributeByName("translation"));
}

BOOST_AUTO_TEST_CASE ( nullptrAttrTest )
{
    auto node = Sg::createNode("/grpa", "group");
    auto attr = Sg::asPtrAttribute(Sg::createAttribute("testptr"));
    auto val = new int(12);
    attr->set(std::exempt_ptr<int>(val));

    node->addAttribute("ptr", std::move(attr));

    BOOST_CHECK (*Sg::asPtrAttribute(node->attributeByName("ptr"))->value() == 12);

//    node->setAttribute("ptr", nullptr);

    Sg::asPtrAttribute(node->attributeByName("ptr"))->set(nullptr);

    BOOST_CHECK (Sg::asPtrAttribute(node->attributeByName("ptr"))->value() == nullptr);
}
