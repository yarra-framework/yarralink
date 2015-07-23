all : release debug i86Release

release: dllRelease
debug:   dllDebug

MAKEFILE_CHAIN += yarralink.mk

#************************************************************************
TARGET_NAME = yarralink
LIB_NAME    = yarralink

#************************************************************************
SOURCES += ${IDEA_BASE}/n4/pkg/MrServers/MrImaging/seq/a_yarra/yarralink.cpp

#************************************************************************
CPPFLAGS_LOCAL += -DFLASH
CPPFLAGS_LOCAL += -DBUILD_SEQU
CPPFLAGS_LOCAL += -DSEQUENCE_CLASS_YARRALINK
CPPFLAGS_LOCAL += -DSEQ_NAMESPACE=SEQ_YARRALINK

#************************************************************************
# Define the software version 
#************************************************************************
#CPPFLAGS_LOCAL += -DVER_VB
CPPFLAGS_LOCAL += -DVER_VD
#CPPFLAGS_LOCAL += -DVER_VE


#************************************************************************
LDLIBS += Sequence.lib
LDLIBS += libSBB.lib
LDLIBS += libSSL.lib
LDLIBS += libRT.lib
LDLIBS += libUILink.lib
LDLIBS += SeqBuffer.lib
LDLIBS += libSeqUtil.lib
LDLIBS += libSeqUT.lib
LDLIBS += libUICtrl.lib
LDLIBS += MrTrace.lib
LDLIBS += MeasNucleiIF.lib
LDLIBS += MrProt.lib
LDLIBS += MeasNucleiBase.lib
LDLIBS += CoilIF.lib
LDLIBS += libPACE.lib
LDLIBS += MeasSections.lib
LDLIBS += PerProxies.lib
LDLIBS += libGSL.lib
LDLIBS += SpuSerGenericComp.lib
LDLIBS += libMES.lib
LDLIBS += MeasPatient.lib
LDLIBS += MdhProxy.lib
LDLIBS += MrBasicObjects.lib

#************************************************************************
include $(IDEA_BASE)/n4/pkg/MrApplications/MrIDEA/utils/MakeSeq.mk
