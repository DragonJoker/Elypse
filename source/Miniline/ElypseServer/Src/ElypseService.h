#ifndef ___ELYPSESERVICE_H___
#define ___ELYPSESERVICE_H___

#include "TcpAcceptor.h"
#include "TcpWriter.h"
#include "TcpReader.h"

#include <Named.h>
#include <memory>

namespace Elypse
{
	namespace ServerPlugin
	{
		enum TypeService
		{
			UdpService = 0,
			TcpService = 1
		};

		class ElypseService
			: public General::Theory::named
		{
		protected:
			ElypseService( const std::string & p_name )
				: General::Theory::named( p_name )
			{
			}

		public:
			virtual ~ElypseService()
			{
			}

		public:
			virtual unsigned short GetPortNo() const = 0;
			virtual TypeService GetTypeService() const = 0;
		};

		class ElypseTcpService
			: public ElypseService
			, public Network::TcpAcceptor
		{
		protected:
			ElypseTcpService( const std::string & p_name, unsigned short p_port )
				: ElypseService( p_name )
				, Network::TcpAcceptor( p_port )
			{
				std::cout << "ElypseTCPService(" << p_name << ") created" << std::endl;
			}

		public:
			virtual ~ElypseTcpService()
			{
				std::cout << "~ElypseTCPService(" << m_name << ") deleted" << std::endl;
			}

		public:
			inline TypeService GetTypeService() const
			{
				return TcpService;
			}
			inline unsigned short GetPortNo() const
			{
				return m_acceptor.local_endpoint().port();
			}
		};

		class ElypseUdpService
			: public ElypseService
		{
		private:
			unsigned short m_port;

		protected:
			ElypseUdpService( const std::string & p_name, unsigned short p_port )
				: ElypseService( p_name )
				, m_port( p_port )
			{
				std::cout << "ElypseUDPService(" << p_name << ") created" << std::endl;
			}

		public:
			virtual ~ElypseUdpService()
			{
				std::cout << "~ElypseUDPService(" << m_name << ") deleted" << std::endl;
			}

		public:
			inline TypeService GetTypeService() const
			{
				return UdpService;
			}
			inline unsigned short GetPortNo() const
			{
				return m_port;
			}
		};

		typedef std::vector< std::shared_ptr< ElypseService > > ElypseServiceArray;
	}
}

#endif
