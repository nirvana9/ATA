#ifndef _IVIBASE_HPP
#define _IVIBASE_HPP

#if defined(__cplusplus) || defined(__cplusplus__)  
#include "iviexcpt.hpp"

// Redefine VI_TRUE and VI_FALSE with the ViBoolean casts so the they do cause an
// ambiguous call error when passed to SetAttribute.
#undef VI_TRUE
#undef VI_FALSE
#define VI_TRUE		((ViBoolean) 1)
#define VI_FALSE	((ViBoolean) 0)

////////////////////////////////////////////////////////////////////////////////
//
//	IviBase
//	
//	This class is the base class for all IVI drivers.  It can not be 
//	instantiated.  This class can be replaced to allow for customized exception
//	handling.
//
//  This m_constructionStatus member is used to hold the error code from a 
//	derived class's constructor when the constructor fails and m_throwExceptions 
//	is VI_FALSE.
//

class IviBase
	{
public:
	virtual				~IviBase (void);

	virtual ViStatus	InitialStatus (void);

	virtual ViSession	GetSession (void);
	virtual ViBoolean	HandleError (ViStatus status);
	
protected:
						IviBase (ViSession vi, ViBoolean throwExceptions);
						IviBase (ViBoolean throwExceptions);
	virtual ViStatus	ExceptionFilter (ViStatus status);
	
    ViSession			m_vi;
	ViBoolean			m_ownsSession;
	ViBoolean			m_throwExceptions;
	ViStatus			m_constructionStatus;	

private:
						// disallow assignment and copying 
						IviBase (const IviBase &instr);			
						IviBase& operator= (const IviBase &instr);			
	};

////////////////////////////////////////////////////////////////////////////////
//
//	Member implementation
//
inline IviBase::IviBase (ViSession vi, ViBoolean throwExceptions)
	{
	m_ownsSession = false;
	m_throwExceptions = throwExceptions;
	m_constructionStatus = VI_SUCCESS;
	m_vi = vi;
	}

inline IviBase::IviBase (ViBoolean throwExceptions)
	{
	m_ownsSession = true;
	m_throwExceptions = throwExceptions;
	m_constructionStatus = VI_SUCCESS;
	m_vi = 0;
	}

inline IviBase::~IviBase (void)
	{
	}

inline ViStatus IviBase::InitialStatus (void)
	{
	return m_constructionStatus;
	}

inline ViSession IviBase::GetSession (void)
	{
	return m_vi;
	}

inline ViStatus IviBase::ExceptionFilter (ViStatus status)
	{
	if (m_vi == 0 && status == 0xBFFF000E /* invalid handle */)
		status = IVI_ERROR_NOT_INITIALIZED;

	if (status < 0) 
		{
		if (!HandleError (status) && m_throwExceptions)
			throw IviException(m_vi, status);
		}

	return status;
	}

inline ViBoolean IviBase::HandleError (ViStatus status)
	{
	return VI_FALSE;
	}

inline IviBase::IviBase (const IviBase &iviBase)
	{
	}

inline IviBase& IviBase::operator= (const IviBase &iviBase)
	{
	return *this;
	}

#endif /* defined(__cplusplus) || defined(__cplusplus__)  */
#endif /* ndef _IVIBASE_H */
