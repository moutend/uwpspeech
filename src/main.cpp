#include <fstream>
#include <iostream>
#include <ppltasks.h>
#include <robuffer.h>
#include <wrl.h>

using namespace Microsoft::WRL;
using namespace Windows::Media::SpeechSynthesis;
using namespace concurrency;
using namespace Windows::Storage::Streams;
using namespace Windows::Media;

using Windows::Foundation::Metadata::ApiInformation;

byte *getBytes(IBuffer ^ buffer) {
  ComPtr<IInspectable> i = reinterpret_cast<IInspectable *>(buffer);
  ComPtr<IBufferByteAccess> bufferByteAccess;

  if (FAILED(i.As(&bufferByteAccess))) {
    return nullptr;
  }

  byte *bs{nullptr};
  bufferByteAccess->Buffer(&bs);

  return bs;
}

int main(Platform::Array<Platform::String ^> ^ args) {
  auto synth = ref new SpeechSynthesizer();

  if (ApiInformation::IsApiContractPresent(
          "Windows.Foundation.UniversalApiContract", 6, 0)) {
    auto options = synth->Options;
    options->AppendedSilence = SpeechAppendedSilence::Min;
  }

  task<SpeechSynthesisStream ^> speechTask;
  Platform::String ^ ssml =
      "<speak version='1.0' "
      "xmlns='http://www.w3.org/2001/10/synthesis' xml:lang='en-US'>"
      "Hello <prosody contour='(0%,+80Hz) (10%,+80%) "
      "(40%,+80Hz)'>World</prosody>"
      "<break time='500ms' /> "
      "Goodbye <prosody rate='slow' contour='(0%,+20Hz) (10%,+30%) "
      "(40%,+10Hz)'>World</prosody>"
      "</speak>";

  try {
    speechTask = create_task(synth->SynthesizeSsmlToStreamAsync(ssml));
  } catch (Platform::Exception ^ e) {
    std::wcerr << L"Failed to create speech task: " << e->Message->Data()
               << std::endl;
    return -2;
  }
  if (speechTask.is_done()) {
    std::cerr << "Failed to continue: check SSML is valid" << std::endl;
    return -3;
  }

  constexpr char *outputFileName{"output.wav"};
  unsigned int length{0};

  speechTask
      .then([&length](SpeechSynthesisStream ^ speechStream) {
        length = static_cast<unsigned int>(speechStream->Size);
        Buffer ^ buffer = ref new Buffer(length);
        auto result = create_task(speechStream->ReadAsync(
            buffer, length,
            Windows::Storage::Streams::InputStreamOptions::None));

        return result;
      })
      .then([&length, outputFileName](IBuffer ^ buffer) {
        byte *bs = getBytes(buffer);
        std::ofstream out(outputFileName, std::ios::binary | std::ios::out);

        if (!out.is_open()) {
          std::cerr << "Failed to open file: " << outputFileName << std::endl;
          return;
        }

        out.write(reinterpret_cast<const char *>(bs),
                  std::streamsize(length * sizeof(byte)));
        out.close();
      })
      .then([](task<void> previous) {
        try {
          previous.get();
        } catch (Platform::Exception ^ e) {
          std::wcerr << L"Failed to complete: " << e->Message->Data()
                     << std::endl;
        }
      })
      .wait();

  return 0;
}
