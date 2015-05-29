#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "bsd_string.h"

#include "genesis.h"
#include "g59cmd.h"

static G59 m_g59;
static G11 m_g11;
static Genesis *mp_genesis = NULL;

static char m_name_str[256];
static char m_model_str[256];
static char m_mode = '\0';
static long m_tune_freq = 1800000;
static long m_lo_freq = 1800000;


#pragma argused
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
	return 1;
}

//Foward Decl
//int SetHWLO(long LOfreq);

extern "C"
void __stdcall __declspec(dllexport) ShowGUI()
{
    fprintf(stderr,"%s:%d\n",__FUNCTION__,__LINE__);
}

extern "C"
bool __stdcall __declspec(dllexport) InitHW(char *name, char *model, int& type)
{
 fprintf(stderr,"%s:%d\n",__FUNCTION__,__LINE__);
    bool rtn = false;
    type = 0;
    name = NULL;
    model = NULL;
    if (rtn = m_g59.Init())
    {
        mp_genesis = &m_g59;
        strlcpy(m_name_str, m_g59.GetMake().c_str(), sizeof(m_name_str));
        strlcpy(m_model_str, m_g59.GetMake().c_str(), sizeof(m_model_str));
        name = m_name_str;
        model = m_model_str;
        type = 4;
    }
    else if (rtn = m_g11.Init())
    {
        mp_genesis = &m_g11;
        strlcpy(m_name_str, m_g11.GetMake().c_str(), sizeof(m_name_str));
        strlcpy(m_model_str, m_g11.GetMake().c_str(), sizeof(m_model_str));
        name = m_name_str;
        model = m_model_str;
        type = 4;
    }

    return rtn;
}

extern "C"
bool __stdcall __declspec(dllexport) OpenHW()
{
	int SetHWLO(long LOfreq);
    fprintf(stderr,"%s:%d\n",__FUNCTION__,__LINE__);
    SetHWLO(m_lo_freq);
    return true;
}

extern "C"
void __stdcall __declspec(dllexport) CloseHW()
{
    fprintf(stderr,"%s:%d\n",__FUNCTION__,__LINE__);
    mp_genesis->Close();
}

extern "C"
int __stdcall __declspec(dllexport) StartHW(long freq)
{
    fprintf(stderr,"%s:%d\n",__FUNCTION__,__LINE__);
    return 0;
}

extern "C"
void __stdcall __declspec(dllexport) StopHW()
{
    fprintf(stderr,"%s:%d\n",__FUNCTION__,__LINE__);
}

extern "C"
int __stdcall __declspec(dllexport) SetHWLO(long LOfreq)
{
    fprintf(stderr,"%s:%d LOfreq: %ld\n",__FUNCTION__,__LINE__, LOfreq);
    mp_genesis->SetLO(LOfreq);
    m_lo_freq = LOfreq;
    return 0;
}

extern "C"
int  __stdcall __declspec(dllexport) GetStatus()
{
    fprintf(stderr,"%s:%d\n",__FUNCTION__,__LINE__);
    return 0;
}

extern "C"
void __stdcall __declspec(dllexport) SetCallback(void (* Callback)(int, int, float, void *))
{
    fprintf(stderr,"%s:%d\n",__FUNCTION__,__LINE__);
}

// ext routs
extern "C"
long __stdcall __declspec(dllexport) GetHWLO()
{
    fprintf(stderr,"%s:%d\n",__FUNCTION__,__LINE__);
    return m_lo_freq;
}

extern "C"
void __stdcall __declspec(dllexport) TuneChanged(long freq)
{
    fprintf(stderr,"%s:%d freq: %ld\n",__FUNCTION__,__LINE__, freq);
    mp_genesis->SetBand(freq);
    m_tune_freq = freq;
}

extern "C"
void __stdcall __declspec(dllexport) IFLimitsChanged(long low, long high)
{
    long LOfreq = (low + high)/2;
    fprintf(stderr,"%s:%d low: %ld, high: %ld, LO = %ld\n",__FUNCTION__,__LINE__, low, high, LOfreq);
    if((0 < LOfreq) && (LOfreq != m_lo_freq))
    {
        mp_genesis->SetLO(LOfreq);
        m_lo_freq = LOfreq;
    }

}

extern "C"
long __stdcall __declspec(dllexport) GetTune()
{
    fprintf(stderr,"%s:%d\n",__FUNCTION__,__LINE__);
    return m_tune_freq;
}

extern "C"
char __stdcall __declspec(dllexport) GetMode()
{
    fprintf(stderr,"%s:%d\n",__FUNCTION__,__LINE__);
    return m_mode;
}

extern "C"
void __stdcall __declspec(dllexport) ModeChanged(char mode)
{
    m_mode = mode;
    fprintf(stderr,"%s:%d %c\n",__FUNCTION__,__LINE__,mode);
}

extern "C"
long __stdcall __declspec(dllexport) GetHWSR()
{
    fprintf(stderr,"%s:%d\n",__FUNCTION__,__LINE__);
    return 0;
}

extern "C"
void __stdcall __declspec(dllexport) HideGUI()
{
    fprintf(stderr,"%s:%d\n",__FUNCTION__,__LINE__);
}

extern "C"
void __stdcall __declspec(dllexport) RawDataReady(long samprate, int *Ldata, int *Rdata, int numsamples)
{
    fprintf(stderr,"%s:%d samprate:%ld, numsamples:%d\n",__FUNCTION__,__LINE__, samprate, numsamples);
}

extern "C"
void __stdcall __declspec(dllexport) GetFilters(int& loCut, int& hiCut, int& pitch)
{
    fprintf(stderr,"%s:%d\n",__FUNCTION__,__LINE__);
}
//
extern "C"
int __stdcall __declspec(dllexport) SetModeRxTx(int modeRxTx)
{
    fprintf(stderr,"%s:%d modeRxTx: %d\n",__FUNCTION__,__LINE__, modeRxTx);
    mp_genesis->SetTx((0!=modeRxTx?true:false));
    return 0;
}

extern "C"
int __stdcall __declspec(dllexport) ActivateTx(int magicA, int magicB)
{
    fprintf(stderr,"%s:%d magicA:%d, int magicB:%d\n",__FUNCTION__,__LINE__,magicA, magicB);
    return 0;
}

extern "C"
void __stdcall __declspec(dllexport) VersionInfo(const char *name, int ver_major, int ver_minor)
{
    fprintf(stderr,"%s:%d name:\"%s\", ver_major:%d, ver_minor:%d\n",__FUNCTION__,__LINE__, name, ver_major, ver_minor);
}

extern "C"
void __stdcall __declspec(dllexport) SetPreamp(int db)
{
    fprintf(stderr,"%s:%d db=%d\n",__FUNCTION__,__LINE__, db);
    if (0 > db)
    {
        mp_genesis->SetAtten(true);
        mp_genesis->SetRFPreamp(false);
    }
    else if ( 0 < db)
    {
        mp_genesis->SetAtten(false);
        mp_genesis->SetRFPreamp(true);
    }
    else
    {
        mp_genesis->SetAtten(false);
        mp_genesis->SetRFPreamp(false);
    }
}
