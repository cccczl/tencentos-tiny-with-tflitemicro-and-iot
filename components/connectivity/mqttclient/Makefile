###################################################################
#automatic detection QTOP and LOCALDIR
CUR_DIR := $(patsubst %/,%,$(dir $(realpath $(firstword $(MAKEFILE_LIST)))))
TRYQTOP := $(shell if [ -n "$$QTOP" ] ; then\
        echo $$QTOP;\
        else\
        cd $(CUR_DIR); while /usr/bin/test ! -d qmk ; do \
        dir=`cd ../;pwd`;                       \
        if [ "$$dir" = "/" ] ; then             \
           echo Cannot find QTOP in $(firstword $(MAKEFILE_LIST)) 1>&2; \
           exit 1;                              \
        fi ;                                    \
        cd $$dir;                               \
        done ;                                  \
        pwd;                                    \
        fi)
QTOP ?= $(realpath ${TRYQTOP})

ifeq ($(QTOP),)
$(error Please run this in a tree)
endif
LOCALDIR = $(patsubst %/,%,$(subst $(realpath $(QTOP))/,,$(CUR_DIR)))

####################################################################


TREE_LIB_ENABLE=y
lib=
subdirs=

SUBDIRS_FIND=$(patsubst ./%,%,$(shell find . -type d))
CFGFLAGS += -std=c99
CFGFLAGS += -DSALOF_USING_TENCENTOS
CFGFLAGS += $(addprefix -I$(CUR_DIR)/, $(SUBDIRS_FIND))


include ${QTOP}/qmk/generic/Make.tpl


