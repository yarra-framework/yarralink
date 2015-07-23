#ifndef yarralink_h
#define yarralink_h 1

#define YL_VERSION "0.1b"


#define PM_USE_SEQIF

#include "MrServers/MrMeasSrv/SeqFW/libSSL/libSSL.h"
#include "MrServers/MrMeasSrv/MeasUtils/nlsmac.h" 
#include "MrServers/MrMeasSrv/SeqIF/Sequence/SeqIF.h"
#ifndef VER_VB20
	#include "MrServers/MrImaging/libSBB/StdSeqIF.h"
#endif
#include "MrServers/MrImaging/libSBB/SEQLoop.h"  

#ifndef VER_VE
    #include "MrServers/MrMeasSrv/SeqIF/sde_allincludes.h"
#endif


#ifdef BUILD_SEQU
    #define __OWNER
#endif
#include "MrCommon/MrGlobalDefinitions/ImpExpCtrl.h"

#ifdef WIN32
    #include <afx.h>                      
#endif

#ifdef WIN32
    #include "MrServers/MrProtSrv/MrProtocol/UILink/MrStdNameTags.h"
    #include "MrServers/MrProtSrv/MrProtocol/libUILink/UILinkLimited.h"
    #include "MrServers/MrProtSrv/MrProtocol/libUILink/UILinkSelection.h"
    #include "MrServers/MrProtSrv/MrProtocol/libUILink/UILinkArray.h"
    #include "MrServers/MrProtSrv/MrProtocol/UILink/StdProtRes/StdProtRes.h"
    #include "MrServers/MrMeasSrv/SeqIF/Sequence/Sequence.h"
	#include "MrServers/MrProtSrv/MrProtocol/libUICtrl/UICtrl.h"
    #include <vector>
#endif


class MrProt;
class SeqLim;
class SeqExpo;
class Sequence;

namespace MrProtocolData
{
    class MrProtData;
}


namespace SEQ_NAMESPACE
{
#ifndef VER_VB20
	class yarralink : public StdSeqIF
#else
	class yarralink : public SeqIF
#endif	
	{
		public:

			yarralink();
			virtual ~yarralink();

			// ----------------------------------------------------------------------------------
			// VB line interface
			// ----------------------------------------------------------------------------------    		
			virtual NLSStatus initialize(SeqLim * pSeqLim);
			virtual NLSStatus prepare(MrProt *pMrProt, SeqLim *pSeqLim, SeqExpo *pSeqExpo);
			virtual NLSStatus check(MrProt * pMrProt, SeqLim *pSeqLim, SeqExpo *pSeqExpo, SEQCheckMode *pSEQCheckMode);
			virtual NLSStatus run(MrProt *pMrProt, SeqLim *pSeqLim, SeqExpo *pSeqExpo);
			virtual NLS_STATUS runKernel(MrProt *pMrProt,SeqLim *pSeqLim, SeqExpo *pSeqExpo, long lKernelMode, long lSlice, long lPartition, long lLine);

			// ----------------------------------------------------------------------------------
			// VD/VE line interface
			// ----------------------------------------------------------------------------------        
			#ifndef VER_VB
				virtual NLSStatus initialize (SeqLim &rSeqLim);
				virtual NLSStatus prepare (MrProt &rMrProt, SeqLim &rSeqLim, SeqExpo &rSeqExpo);
				virtual NLSStatus check (MrProt &rMrProt, SeqLim &rSeqLim, SeqExpo &rSeqExpo, SEQCheckMode *  pSEQCheckMode);
				virtual NLSStatus run (MrProt &rMrProt, SeqLim &rSeqLim, SeqExpo &rSeqExpo);
				virtual NLS_STATUS runKernel (MrProt &rMrProt, SeqLim &rSeqLim, SeqExpo &rSeqExpo, long lKernelMode, long lSlice, long lPartition, long lLine);
			#endif
			
		protected:	
			bool clientLaunched;
	};
}

#endif // yarralink_h
