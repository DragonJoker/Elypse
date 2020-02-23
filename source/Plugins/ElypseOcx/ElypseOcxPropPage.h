#pragma once

// ElypseOcxPropPage.h : Declaration of the CElypseOcxPropPage property page class.
class CElypseOcxPropPage
	: public COlePropertyPage
{
	DECLARE_DYNCREATE( CElypseOcxPropPage )
	DECLARE_OLECREATE_EX( CElypseOcxPropPage )

public:
	CElypseOcxPropPage();

	enum { IDD = IDD_PROPPAGE_ELYPSEPLAYER };

protected:
	virtual void DoDataExchange( CDataExchange * pDX );
	DECLARE_MESSAGE_MAP()
};

