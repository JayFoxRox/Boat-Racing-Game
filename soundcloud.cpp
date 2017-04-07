#include <curl/curl.h>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iterator>

class SoundCloud {

  protected:

  //FIXME: Stolen from random repo on GitHub
  static const std::string clientId;

  static size_t receiveFunction(void *ptr, size_t size, size_t nmemb, std::vector<uint8_t>* s) {
    size_t length = size * nmemb;
    uint8_t* byte = (uint8_t*)ptr;
    while(length--) {
#if 0
      std::cout << "Receiving! " << (char)*byte << std::endl;
#endif
      s->push_back(*byte++);
    }
    return size * nmemb;
  }

  static std::vector<uint8_t> request(std::string url) {
    CURL *curl;
    CURLcode res;
   
    std::vector<uint8_t> data;

    curl = curl_easy_init();
    if(curl) {
      std::cout << "Parsing: " << url << std::endl;

      curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
      /* example.com is redirected, so we tell libcurl to follow redirection */ 
      curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
   
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, receiveFunction);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);

      /* Perform the request, res will get the return code */ 
      res = curl_easy_perform(curl);
      /* Check for errors */ 
      if(res != CURLE_OK) {
        std::cerr << std::string("curl_easy_perform() failed: ") << std::string(curl_easy_strerror(res)) << std::endl;
      }
   
      /* always cleanup */ 
      curl_easy_cleanup(curl);
    }
  
    return data;
  }

  public:

  // Returns a trackId
  static uint64_t resolveTrackUrl(std::string url) {
    auto data = request(std::string("http://api.soundcloud.com/resolve.json?url=") + url + std::string("&client_id=") + clientId);
    //FIXME: Assert that "kind" is "track"
    std::string str(data.begin(), data.end());
    std::cout << "Received: <" << str << ">" << std::endl;

    return 225965238; //FIXME: data->id;
  }
    
  class Track {
    private:
    uint64_t trackId;
    public:
    Track(uint64_t trackId) : trackId(trackId) {}
    void download() {
      std::cout << "Downloading track " << trackId << std::endl;

      // Download 128kbs mp3 stream
      auto track = request(std::string("https://api.soundcloud.com/tracks/") + std::to_string(trackId) + std::string("/stream?client_id=") + clientId);
      
      std::string fileName = std::to_string(trackId) + ".mp3";
      std::cout << "Will export to " << fileName << std::endl;

      std::ofstream file;
      file.open(fileName);
      std::copy(track.begin(), track.end(), std::ostream_iterator<uint8_t>(file));
      file.close();

      std::cout << "Complete" << std::endl;
    }
  };

};
const std::string SoundCloud::clientId("fc6924c8838d01597bab5ab42807c4ae");



