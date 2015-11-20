lib_LTLIBRARIES            += libsgcore.la

libsgcore_la_SOURCES        =                                   \
                              src/core/timer.cpp                \
                              src/core/object.cpp               \
                              src/core/attribute.cpp            \
                              src/core/master.cpp               \
                              src/core/global.cpp               \
                              src/core/dagManager.cpp           \
                              src/core/eventManager.cpp         \
                              src/core/driver.cpp               \
                              src/core/node.cpp                 \
                              src/core/nameTree.cpp             \
                              src/core/nodeManager.cpp          \
                              src/core/relation.cpp             \
                              src/core/relationship.cpp         \
                              src/core/relationshipType.cpp     \
                              src/core/relationshipManager.cpp  \
                              src/core/selectionManager.cpp     \
                              src/core/timestamp.cpp            \
                              src/core/ostreamhelpers.cpp
                              
libsgcoreinclude_HEADERS    =                                   \
                              src/core/classPointers.h          \
                              src/core/logging.h                \
                              src/core/exception.h              \
                              src/core/constants.h              \
                              src/core/master.h                 \
                              src/core/global.h                 \
                              src/core/ostreamhelpers.h         \
                              src/core/timestamp.h              \
                              src/core/genericany.h             \
                              src/core/tree.hh                  \
                                                                \
                              src/core/object.h                 \
                              src/core/node.h                   \
                              src/core/nameTree.h               \
                              src/core/dagManager.h             \
                                                                \
                              src/core/attribute.h              \
                              src/core/typedAttribute.h         \
                              src/core/standardAttributes.h     \
                              src/core/boolAttribute.h          \
                              src/core/intAttribute.h           \
                              src/core/uintAttribute.h          \
                              src/core/floatAttribute.h         \
                              src/core/stringAttribute.h        \
                                                                \
                              src/core/eventManager.h           \
                              src/core/selectionManager.h       \
                              src/core/relationshipManager.h    \
                              src/core/nodeManager.h            \
                              src/core/nodeFactory.h            \
                              src/core/attributeFactory.h       \
                              src/core/driver.h                 \
                              src/core/functor.h                \
                              src/core/lambdaFunctor.h          \
                                                                \
                              src/core/relation.h               \
                              src/core/relationship.h           \
                              src/core/relationshipType.h

libsgcoreincludedir         = $(includedir)/sg/core
                           
libsgcore_la_LDFLAGS        = -ldl -version-info @LIBRARY_VERSION@ -no-undefined

libsgcore_la_CPPFLAGS       =                                   \
                              -I/home/jasonr/workspace/librcf/build-dbg-amd64/install/include \
                              -I$(top_builddir)                 \
                              -I$(top_srcdir)                   \
                              -I$(top_srcdir)/src               \
                              @LIBUT_CFLAGS@                    \
                              @LIBLOG_CFLAGS@                   \
                              @ILMBASE_CFLAGS@                  \
                              @BOOST_CPPFLAGS@
