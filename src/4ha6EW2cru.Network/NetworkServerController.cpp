#include "NetworkServerController.h"

#include "NetworkClientEndpoint.h"
#include "NetworkStream.h"
using namespace RakNet;

#include "IO/IStream.hpp"
using namespace IO;

#include "Management/Management.h"

namespace Network
{
	NetworkServerController::~NetworkServerController( )
	{

	}

	void NetworkServerController::Initialize( )
	{
		RPC3_REGISTER_FUNCTION( m_networkInterface->GetRPC( ), &NetworkClientEndpoint::Net_LoadLevel );
		RPC3_REGISTER_FUNCTION( m_networkInterface->GetRPC( ), &NetworkClientEndpoint::Net_UpdateWorld );
		RPC3_REGISTER_FUNCTION( m_networkInterface->GetRPC( ), &NetworkClientEndpoint::Net_CreateEntity );
		RPC3_REGISTER_FUNCTION( m_networkInterface->GetRPC( ), &NetworkClientEndpoint::Net_DestroyEntity );
		RPC3_REGISTER_FUNCTION( m_networkInterface->GetRPC( ), &NetworkClientEndpoint::Net_SetEntityPosition );		
	}

	void NetworkServerController::ClientConnected( const SystemAddress& clientAddress )
	{
		m_networkInterface->GetRPC( )->SetRecipientAddress( clientAddress, false );
		m_networkInterface->GetRPC( )->CallC( "&NetworkClientEndpoint::Net_LoadLevel", RakString( Management::Get( )->GetInstrumentation( )->GetLevelName( ) ) );
	}

	void NetworkServerController::CreateEntity( const std::string& entityName, const std::string& entityType )
	{
		m_networkInterface->GetRPC( )->SetRecipientAddress( UNASSIGNED_SYSTEM_ADDRESS, true );
		m_networkInterface->GetRPC( )->CallC( "&NetworkClientEndpoint::Net_CreateEntity", RakString( entityName ), RakString( entityType ) );
	}

	void NetworkServerController::DestroyEntity( const std::string& entityName )
	{
		m_networkInterface->GetRPC( )->SetRecipientAddress( UNASSIGNED_SYSTEM_ADDRESS, true );
		m_networkInterface->GetRPC( )->CallC( "NetworkClientEndpoint::Net_DestroyEntity", RakString( entityName ) );
	}

	void NetworkServerController::SendWorldUpdate( const SystemAddress& address )
	{
		m_networkInterface->GetRPC( )->SetRecipientAddress( address, false );
		
		BitStream stream;
		NetworkStream networkStream( &stream );

		AnyType::AnyTypeMap parameters;
		parameters[ System::Parameters::IO::Stream ] = static_cast< IStream* >( &networkStream );

		m_serviceManager->FindService( System::Types::ENTITY )
			->ProcessMessage( System::Messages::Entity::SerializeWorld, parameters );

		m_networkInterface->GetRPC( )->CallC( "&NetworkClientEndpoint::Net_UpdateWorld", stream );
	}

	void NetworkServerController::ClientDisconnected( const SystemAddress& clientAddress )
	{
		AnyType::AnyTypeMap parameters;
		parameters[ System::Attributes::Name ] = std::string( clientAddress.ToString( ) );

		m_serviceManager->FindService( System::Types::ENTITY )
			->ProcessMessage( System::Messages::Entity::DestroyEntity, parameters );
	}

	void NetworkServerController::SetEntityPosition( const std::string& entityName, const Maths::MathVector3& position )
	{
		m_networkInterface->GetRPC( )->CallC( "&NetworkClientEndpoint::Net_SetEntityPosition", RakString( entityName ), position );
	}
}