#pragma once

#include "ElypseOcxPlugin.h"

#include <Module_Main.h>

#include <Path.h>
#include <Singleton.h>
#include <FunctionTracker.h>

// {ED75F1E4-D316-42CA-A48C-3A0F04F0D746}
const IID BASED_CODE IID_ElypsePlayerDispatch =
{ 0XED75F1E4, 0XD316, 0X42CA, { 0XA4, 0X8C, 0X3A, 0X0F, 0X4, 0XF0, 0XD7, 0X46 } };

// {59EA608D-5F7C-4EDE-B72C-A88B587DDB34}
const IID BASED_CODE IID_ElypsePlayerEvent =
{ 0X59EA608D, 0X5F7C, 0X4EDE, { 0XB7, 0X2C, 0XA8, 0X8B, 0X58, 0X7D, 0XDB, 0X34 } };

//The singleton isn't used as a singleton, it's used to dispatch keyboard / mouse messages to the right instance.
class ElypseOcxCtrl
	: public COleControl
	, public General::Theory::Singleton< ElypseOcxCtrl >
{
private:
	DECLARE_DYNCREATE( ElypseOcxCtrl )

public:
	ElypseOcxCtrl();
	virtual void DoPropExchange( CPropExchange * pPX );
	virtual void OnResetState();
	DWORD ElypseOcxCtrl::GetControlFlags();

public:
	inline HHOOK GetHook()const
	{
		return m_hook;
	}
	static LRESULT CALLBACK MessageProc( int nCode, WPARAM wParam, LPARAM lParam );

protected:
	~ElypseOcxCtrl();
	HRESULT DoExecuteScript( char * p_text );
	void DoCheckInstall();
	String DoGetProperty( const String & p_propertyName, CPropExchange * p_properties );
	void DoGetClientSite();
	void DoUpdatePlayer();
	void DoInitialise();
	void DoActivateDebugConsole();

private:
	BEGIN_OLEFACTORY( ElypseOcxCtrl )		// Class factory and guid
	END_OLEFACTORY( ElypseOcxCtrl )
	DECLARE_OLETYPELIB( ElypseOcxCtrl )		// GetTypeInfo
	DECLARE_PROPPAGEIDS( ElypseOcxCtrl )	// Property page IDs
	DECLARE_OLECTLTYPE( ElypseOcxCtrl )		// Type name and misc status

	DECLARE_MESSAGE_MAP()
	afx_msg	int OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg void OnActivate( UINT nState, CWnd * pWndOther, BOOL bMinimized );
	afx_msg void OnKillFocus( CWnd * pOldWnd );
	afx_msg void OnSetFocus( CWnd * pOldWnd );
	afx_msg void OnDestroy();
	afx_msg void OnMouseMove( UINT nHitTest, CPoint point );
	afx_msg void OnLButtonDown( UINT, CPoint );
	afx_msg void OnLButtonDblClk( UINT, CPoint );
	afx_msg void OnLButtonUp( UINT, CPoint );
	afx_msg void OnRButtonDown( UINT, CPoint );
	afx_msg void OnRButtonDblClk( UINT, CPoint );
	afx_msg void OnRButtonUp( UINT, CPoint );
	afx_msg void OnMButtonDown( UINT, CPoint );
	afx_msg void OnMButtonDblClk( UINT, CPoint );
	afx_msg void OnMButtonUp( UINT, CPoint );
	afx_msg void OnSysKeyUp( UINT, UINT, UINT );
	afx_msg void OnSysKeyDown( UINT, UINT, UINT );
	afx_msg void OnKeyUp( UINT, UINT, UINT );
	afx_msg void OnKeyDown( UINT, UINT, UINT );
	afx_msg BOOL OnMouseWheel( UINT, short, CPoint );
	afx_msg void OnPaint();
	afx_msg void OnChar( UINT, UINT, UINT );
	afx_msg void OnDeadChar( UINT, UINT, UINT );
	afx_msg LRESULT OnExecJS( WPARAM, LPARAM );


	DECLARE_DISPATCH_MAP()
	DECLARE_EVENT_MAP()

	void OnDraw( CDC *, const CRect &, const CRect & );

private:
	bool m_activeDebug;
	bool m_leftButton;
	bool m_rightButton;
	bool m_middleButton;
	bool m_ogreActive;
	bool m_secondProp;
	bool m_noInit;
	bool m_destroyed;
	bool m_oneTimePostSetFocus;
	Real m_height;
	Real m_width;
	String m_baseDirectory;
	String m_linkedTo;
	Path m_installPath;
	Elypse::OCX::ElypseOcxPlugin * m_plugin;
	bool m_focused;
	bool m_primary;
	bool m_deleteAll;
	ElypseInstance * m_ogre;
	HHOOK m_hook;
};

