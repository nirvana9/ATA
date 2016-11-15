#ifndef _IVIEXCEPT_HPP
#define _IVIEXCEPT_HPP

#if defined(__cplusplus) || defined(__cplusplus__)  

#include "ivi.h"

////////////////////////////////////////////////////////////////////////////////
//
//	IviException
//
//	This class is thrown as an exception by an IVI driver class when an error 
//	occurs IF the driver was constructed with the option throwExceptions 
//	parameter set to VI_TRUE.  The default value of throwExceptions is VI_FALSE. 
//	In this case, driver errors can be detected by checking the return values of 
//	the driver's member functions.	This gives the user of an IVI driver class 
//	the choice of detecting errors either by catching exceptions or by the more 
//	traditional method of checking return codes.
//

class IviException
	{
public:
						IviException (ViSession vi, ViStatus status);
						~IviException (void);
	virtual ViSession	GetSession (void);
	virtual void		SetSession (ViSession vi);
	virtual ViStatus	GetStatus (void);
	virtual void		SetStatus (ViStatus status);

private:
	ViStatus			m_status;
	ViSession			m_vi;
	};

////////////////////////////////////////////////////////////////////////////////
//
//	Member implementation

inline IviException::IviException (ViSession vi, ViStatus status)
	{
	m_vi = vi; 
	m_status = status;
	}

inline IviException::~IviException (void)
	{
	}

inline ViSession IviException::GetSession (void)
	{
	return m_vi;
	}

inline void IviException::SetSession (ViSession vi) 
	{
	m_vi = vi;
	}

inline ViStatus IviException::GetStatus (void) 
	{ 
	return m_status;
	}

inline void IviException::SetStatus (ViStatus status) 
	{
	m_status = status;
	}

#endif // defined(__cplusplus) || defined(__cplusplus__)  
#endif // ndef _IVIEXCEPT_HPP