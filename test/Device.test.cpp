#include "Device.hpp"
#include "Exception.hpp"
#include "FakeTransportListener.hpp"
#include "catch.hpp"
#include "data/parameters.hpp"
#include "ortc.hpp"
#include <memory>

TEST_CASE("Device", "[Device]")
{
	static const json TransportRemoteParameters = generateTransportRemoteParameters();

	static FakeSendTransportListener sendTransportListener;
	static FakeRecvTransportListener recvTransportListener;

	static std::unique_ptr<mediasoupclient::Device> device(
	  new mediasoupclient::Device());

	static json routerRtpCapabilities;

	SECTION("device.IsLoaded() is false if not loaded")
	{
		REQUIRE(!device->IsLoaded());
	}

	SECTION("device.GetRtpCapabilities() throws if not loaded")
	{
		REQUIRE_THROWS_AS(device->GetRtpCapabilities(), mediasoupclient::Exception);
	}

	SECTION("device.CanProduce() with audio/video throws if not loaded")
	{
		REQUIRE_THROWS_AS(device->CanProduce("audio"), mediasoupclient::Exception);
		REQUIRE_THROWS_AS(device->CanProduce("video"), mediasoupclient::Exception);
	}

	SECTION("device.CreateSendTransport() fails if not loaded")
	{
		REQUIRE_THROWS_AS(
		  device->CreateSendTransport(
		    &sendTransportListener,
		    TransportRemoteParameters["id"],
		    TransportRemoteParameters["iceParameters"],
		    TransportRemoteParameters["iceCandidates"],
		    TransportRemoteParameters["dtlsParameters"]),
		  mediasoupclient::Exception);
	}

	SECTION("device.CreateRecvTransport() fails if not loaded")
	{
		REQUIRE_THROWS_AS(
		  device->CreateRecvTransport(
		    &recvTransportListener,
		    TransportRemoteParameters["id"],
		    TransportRemoteParameters["iceParameters"],
		    TransportRemoteParameters["iceCandidates"],
		    TransportRemoteParameters["dtlsParameters"]),
		  mediasoupclient::Exception);
	}

	SECTION("device.Load() succeeds")
	{
		routerRtpCapabilities = generateRouterRtpCapabilities();

		REQUIRE_NOTHROW(device->Load(routerRtpCapabilities));
		REQUIRE(device->IsLoaded());
	}

	SECTION("device.Load() fails if already loaded")
	{
		REQUIRE_THROWS_AS(device->Load(routerRtpCapabilities), mediasoupclient::Exception);
	}

	SECTION("device.GetRtpCapabilities() succeeds")
	{
		REQUIRE(device->GetRtpCapabilities().is_object());
	}

	SECTION("device.CanProduce() with 'audio'/'video' kind returns true")
	{
		REQUIRE(device->CanProduce("audio"));
		REQUIRE(device->CanProduce("video"));
	}

	SECTION("device.CanProduce() with invalid kind throws exception")
	{
		REQUIRE_THROWS_AS(device->CanProduce("chicken"), mediasoupclient::Exception);
	}

	SECTION("device.CreateSendTransport() succeeds")
	{
		REQUIRE_NOTHROW(device->CreateSendTransport(
		  &sendTransportListener,
		  TransportRemoteParameters["id"],
		  TransportRemoteParameters["iceParameters"],
		  TransportRemoteParameters["iceCandidates"],
		  TransportRemoteParameters["dtlsParameters"]));
	}

	SECTION("device.CreateRecvTransport() succeeds")
	{
		REQUIRE_NOTHROW(device->CreateRecvTransport(
		  &recvTransportListener,
		  TransportRemoteParameters["id"],
		  TransportRemoteParameters["iceParameters"],
		  TransportRemoteParameters["iceCandidates"],
		  TransportRemoteParameters["dtlsParameters"]));
	}
}
