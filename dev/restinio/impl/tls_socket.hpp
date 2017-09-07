/*
	restinio
*/

/*!
	Socket adapter for asio::ssl::stream< asio::ip::tcp::socket >.
*/

namespace restinio
{

namespace impl
{

//
// tls_socket_t
//

//! Socket adapter for asio::ssl::stream< asio::ip::tcp::socket >.
/*!
	As asio::ssl::stream< asio::ip::tcp::socket > class is not movable
	and lack some some functionality compared to asio::ip::tcp::socket
	it is necesasary to have an adapter for it to use it the same way as
	asio::ip::tcp::socket in template classes and functions.
*/
class tls_socket_t
{
	public:
		using socket_t = asio::ssl::stream< asio::ip::tcp::socket >;
		tls_socket_t( const tls_socket_t & ) = delete;
		const tls_socket_t & operator = ( const tls_socket_t & ) = delete;

		tls_socket_t(
			asio::io_context & io_context,
			asio::ssl::context & tls_context )
			:	m_socket{ std::make_unique< socket_t >( io_context, tls_context ) }
		{}

		tls_socket_t( tls_socket_t && ) = default;
		tls_socket_t & operator = ( tls_socket_t && ) = default;

		void
		swap( tls_socket_t & sock )
		{
			std::swap( m_socket, sock.m_socket );
		}

		auto &
		lowest_layer()
		{
			return m_socket->lowest_layer();
		}

		const auto &
		lowest_layer() const
		{
			return m_socket->lowest_layer();
		}

		auto
		get_executor()
		{
			return lowest_layer().get_executor();
		}

		auto
		remote_endpoint() const
		{
			return lowest_layer().remote_endpoint();
		}

		auto
		is_open() const
		{
			return lowest_layer().is_open();
		}

		template< typename... ARGS >
		void
		cancel( ARGS &&... args )
		{
			lowest_layer().cancel( std::forward< ARGS >( args )... );
		}

		template< typename... ARGS >
		auto
		async_read_some( ARGS &&... args )
		{
			return m_socket->async_read_some( std::forward< ARGS >( args )... );
		}

		template< typename... ARGS >
		auto
		async_write_some( ARGS &&... args )
		{
			return m_socket->async_write_some( std::forward< ARGS >( args )... );
		}

		template< typename... ARGS >
		void
		shutdown( ARGS &&... args )
		{
			lowest_layer().shutdown( std::forward< ARGS >( args )... );
		}

		template< typename... ARGS >
		void
		close( ARGS &&... args )
		{
			lowest_layer().close( std::forward< ARGS >( args )... );
		}

		template< typename... ARGS >
		auto
		async_handshake( ARGS &&... args )
		{
			return m_socket->async_handshake( std::forward< ARGS >( args )... );
		}

	private:
		std::unique_ptr< socket_t > m_socket;
};

} /* namespace impl */

} /* namespace restinio */
