#include "peerConnectionUtils.hpp"
#include "VcmCapturer.hpp"
#include "modules/video_capture/video_capture.h"
#include "modules/video_capture/video_capture_factory.h"
#include "pc/video_track_source.h"
#include "api/audio_codecs/builtin_audio_decoder_factory.h"
#include "api/audio_codecs/builtin_audio_encoder_factory.h"
#include "api/create_peerconnection_factory.h"
#include "api/video_codecs/builtin_video_decoder_factory.h"
#include "api/video_codecs/builtin_video_encoder_factory.h"
#include <iostream>
#include "desktop_capture.h"
#include "ccfg.h"
#include "pc/test/fake_audio_capture_module.h"
#include "pc/test/fake_periodic_video_track_source.h"
#include "pc/test/frame_generator_capturer_video_track_source.h"
#include "system_wrappers/include/clock.h"
#include "api/audio_codecs/builtin_audio_decoder_factory.h"
#include "api/audio_codecs/builtin_audio_encoder_factory.h"
#include "api/create_peerconnection_factory.h"
#include "api/video_codecs/builtin_video_decoder_factory.h"
#include "api/video_codecs/builtin_video_encoder_factory.h"
#include "test/fake_audio_capture_module.h"
static rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface> peerConnectionFactory{ nullptr };

static rtc::scoped_refptr<webrtc::AudioSourceInterface> audioSource{ nullptr };
static rtc::scoped_refptr<webrtc::VideoTrackSourceInterface> videoSource{ nullptr };
static rtc::scoped_refptr<webrtc::VideoCaptureModule> videoCaptureModule;

//class CapturerTrackSource;
//static rtc::scoped_refptr<CapturerTrackSource> videoDevice{ nullptr };
class CapturerTrackSource;
static rtc::scoped_refptr<CapturerTrackSource> videoDevice{ nullptr };


static rtc::Thread* networkThread{ nullptr };
static rtc::Thread* signalingThread{ nullptr };
static rtc::Thread* workerThread{ nullptr };



class CapturerTrackSource : public webrtc::VideoTrackSource
{
public:
	static rtc::scoped_refptr<CapturerTrackSource> Create()
	{
		uint32_t fps = chen::g_cfg.get_int32(chen::ECI_Video_Fps);
		if (fps <= 0)
		{
			fps = 30;
		}
		std::unique_ptr<DesktopCapture> capturer =
			absl::WrapUnique(DesktopCapture::Create(fps, 0));
		if (!capturer)
		{
			return nullptr;
		}
		capturer->StartCapture();
		capturer->AddOrUpdateSink(capturer.get(), rtc::VideoSinkWants());
		return new rtc::RefCountedObject<CapturerTrackSource>(std::move(capturer));
		/*const size_t kWidth       = 1280;
		const size_t kHeight      = 720;
		const size_t kFps         = 30;
		const size_t kDeviceIndex = 0;
		std::unique_ptr<VcmCapturer> capturer =
		  absl::WrapUnique(VcmCapturer::Create(kWidth, kHeight, kFps, kDeviceIndex));
		if (!capturer)
		{
			return nullptr;
		}
		return new rtc::RefCountedObject<CapturerTrackSource>(std::move(capturer));*/
	}
	virtual ~CapturerTrackSource()
	{
		capturer->StopCapture();
	}
	void stop()
	{
		capturer->StopCapture();
	}
protected:
	explicit CapturerTrackSource(std::unique_ptr<DesktopCapture> capturer)
	  : VideoTrackSource(/*remote=*/false), capturer(std::move(capturer))
	{
	}

private:
	rtc::VideoSourceInterface<webrtc::VideoFrame>* source() override
	{
		return capturer.get();
	}
	std::unique_ptr<DesktopCapture> capturer;
};

// PeerConnection factory creation.
static void createPeerConnectionFactory()
{
	std::cout << "[INFO] peerConnectionUtils.createPeerConnectionFactory()" << std::endl;

	webrtc::PeerConnectionInterface::RTCConfiguration config;
	networkThread = new rtc::Thread();
	signalingThread = new rtc::Thread();
	workerThread    = new rtc::Thread();

	networkThread->SetName("network_thread", nullptr);
	signalingThread->SetName("signaling_thread", nullptr);
	workerThread->SetName("worker_thread", nullptr);

	if (!networkThread->Start() ||!signalingThread->Start() || !workerThread->Start())
	{
		throw std::runtime_error("Thread start errored");
	}
	auto fakeAudioCaptureModule = cFakeAudioCaptureModule::Create();
	if (!fakeAudioCaptureModule)
	{
		RTC_LOG(LS_ERROR) << "audio capture module creation errored";
	}
	peerConnectionFactory = webrtc::CreatePeerConnectionFactory(
		networkThread,
	  workerThread,
	  signalingThread,
		fakeAudioCaptureModule /*nullptr*/ /*default_adm*/,
	  webrtc::CreateBuiltinAudioEncoderFactory(),
	  webrtc::CreateBuiltinAudioDecoderFactory(),
	  webrtc::CreateBuiltinVideoEncoderFactory(),
	  webrtc::CreateBuiltinVideoDecoderFactory(),
	  nullptr /*audio_mixer*/,
	  nullptr /*audio_processing*/);
}

// Audio source creation.
static void createAudioSource()
{
	std::cout << "[INFO] peerConnectionUtils.createAudiosource()" << std::endl;

	//cricket::AudioOptions options;

	if (peerConnectionFactory == nullptr)
	{
		createPeerConnectionFactory();
	}
	cricket::AudioOptions options;
	options.highpass_filter = false;
	audioSource = peerConnectionFactory->CreateAudioSource(options/*options*/);
}

// Audio track creation.
rtc::scoped_refptr<webrtc::AudioTrackInterface> createAudioTrack(const std::string& label)
{
	std::cout << "[INFO] peerConnectionUtils.createAudioTrack()" << std::endl;

	if (audioSource == nullptr)
	{
		createAudioSource();
	}

	return peerConnectionFactory->CreateAudioTrack(label, audioSource);
}

// Video track creation.
rtc::scoped_refptr<webrtc::VideoTrackInterface> createVideoTrack(const std::string& label)
{
	std::cout << "[INFO] peerConnectionUtils.createVideoTrack()" << std::endl;

	if (peerConnectionFactory == nullptr)
	{
		createPeerConnectionFactory();
	}

	if (videoDevice == nullptr)
	{
		videoDevice = CapturerTrackSource::Create(); //CapturerTrackSource::Create();
	}

	return peerConnectionFactory->CreateVideoTrack(label, videoDevice);
}


void stopTrack()
{
	RTC_LOG(LS_INFO) << "stop Track ....";
	//CapturerTrackSource* capturer = videoDevice.release();
	if (videoDevice)
	{
		videoDevice->stop();
		RTC_LOG(LS_INFO) << "stop video ok !!!";
	}

	RTC_LOG(LS_INFO) << "stop Track ok !!!";
}
