/*********************************************************************************************************************

	Author :	Sylvain DOREMUS

	Company :	ForDev Studio - Copyright 2006

	Files :		DataWriter.h - DataWriter.cpp

	Desc :		Class used to stream the datas used in EMuse.

*********************************************************************************************************************/
#ifndef CDataStreamer_H
#define CDataStreamer_H

#include <vector>
#include <array>

#include <wx/listbox.h>
#include <wx/protocol/protocol.h>
#include <wx/stream.h>

typedef enum EMBlockType
{
	EM_BLOCK_ZIPDATA		= 1,	/* .zip files */
	EM_BLOCK_GZIPDATA		= 2,	/* unused */
	EM_BLOCK_SCENE			= 3,	/* .emscene files */
	EM_BLOCK_INTERACTIONS	= 4,	/* .emscript files */
	EM_BLOCK_SNDDATA		= 5,	/* unused */
	EM_BLOCK_CONFIG			= 6,	/* unused */
	EM_BLOCK_ZIPSNDDATA		= 7,	/* .emsnd files */
	EM_BLOCK_EMCONFIG		= 8,	/* unused */
	EM_NB_BLOCK_TYPES		= 8,
	EM_BLOCK_ERROR			= 9,
}	EMBlockType;

typedef enum EMSessionType
{
	EM_TYPE_LOCAL = 1,
	EM_TYPE_HTTP = 2,
	EM_TYPE_FTP = 3,
}	EMSessionType;

// The struct to stock blocks informations
struct EM_Block
{
	short m_blocktype;
	short m_namesize;
	wxString m_name;
	int m_blocsize;
	wxString m_hash;
};

typedef std::vector< EM_Block > BlockList;

class CDataStreamer
{
public:
	/**
		Default constructor, does nothing special
	*/
	CDataStreamer( wxListBox * p_log );
	/**
		Default destructor, calls Cleanup()
	*/
	~CDataStreamer();
	/**
		Attempts to connect to the file pointed by p_url in order to stream it
			@params :	p_url :	[in]	the url of the file to stream
			@return :	true if successful
						false if not
	*/
	bool SetFile( wxString const & p_url );
	/**
		Sets m_outFolder to p_url
			@params :	p_url : [in] the new url of the out folder
	*/
	inline void SetOutFolder( wxString const & p_url )
	{
		m_outFolder = p_url;
	}
	/**
		Attempts to read the header of the file, to get the infos of the blocks contained in the file
			@return :	true if successful
						false if not
	*/
	bool ReadHeader();
	/**
		Attempts to get the next block in the file
			@params :	p_buffer : [out]	the buffer used to stock the block into
			@return :	true if successful
						false if not
	*/
	bool GetNextBlock();
	/**
		Function used to delete all pointers
	*/
	void Cleanup();
	/**
		Function used to delete the stream and all dependent objects
	*/
	void CleanupStream();
	/**
		Gives the file name of the block at p_index
			@params :	p_index : [in] index of the block
			@return :	the name of the file
	*/
	wxString GetFileName( int p_index );

private:
	//! Input stream (can be ftp, http or local file)
	wxInputStream * m_stream;
	//! The stream socket (for ftp or http file)
	wxProtocol * m_protocol;
	//! The session type
	int m_type;
	//! The index of the next block to read in the file
	int m_currentBlockIndex;
	//! The number of blocks contained in the file
	int m_nbBlocks;
	//! The output log list box
	wxListBox * m_log;
	BlockList m_header;
	wxString m_outFolder;

private:
	/**
	*@brief
	*	Function which reads p_count bytes from m_ftpFile and puts it into p_buffer
	*@remarks
	*	Throws a std::runtime_error if the value couldn't be retrieved
	*@param[in,out] p_buffer
	*	The value to retrieve
	*/
	void Read( uint8_t * p_value, size_t p_size );
	/**
	*@brief
	*	Function which reads p_count bytes from m_ftpFile and puts it into p_buffer
	*@remarks
	*	Throws a std::runtime_error if the value couldn't be retrieved
	*@param[in,out] p_buffer
	*	The value to retrieve
	*/
	template< typename T >
	void Read( T & p_value )
	{
		Read( reinterpret_cast< uint8_t * >( &p_value ), sizeof( T ) );
	}
	/**
		Function that returns the type of session opened
			@params :	p_url : [in]	the url used to determine the type of session
			@return :	the type of connection if known,
						0	if unknown type
						-1	if p_url is not a valid url
	*/
	int GetType( wxString const & p_url );
};

#endif
