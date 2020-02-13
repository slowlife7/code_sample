#ifndef __NXCONFIG_READER_H__
#define __NXCONFIG_READER_H__

#include "types.h"

#define NXPROPERTIES NXConfigReader::instance()
#define NXPROPERTIESLOAD NXConfigReader::instance().LoadConfigure("nxcapi.ini");

using namespace type;
class NXConfigReader
{
public:
	static NXConfigReader& instance();

	void set_io_context_seconds(const uint_t s);
	void set_io_context_useconds(const uint_t us);
	void set_heart_beat_time(const uint_t t);
	void set_log_level(const uint_t lv);
	void set_log_path(const char* path);
	void set_dial_timeout(const uint_t t);
	void set_answer_timeout(const uint_t t);
	void set_sst_timeout(const uint_t t);
	
	uint_t log_level() const;
	std::string log_path() const;
	uint_t hearbeat_time() const;
	uint_t io_context_seconds() const;
	uint_t io_context_useconds() const;
	uint_t dial_timeout() const;
	uint_t answer_timeout() const;
	uint_t sst_timeout() const;

	void LoadConfigure(const char* const file_name);

	static const char* const kHeartBeat;
	static const char* const kIoContextSecond;
	static const char* const kIoContextMicroSecond;
	static const char* const kLogLevel;
	static const char* const kLogPath;
	static const char* const kDialTimeOut;
	static const char* const kSSTTimeOut;
	static const char* const kAnswerTimeOut;

private:
	NXConfigReader();
	~NXConfigReader();
	NXConfigReader(const NXConfigReader&);
	NXConfigReader& operator=(const NXConfigReader&);

	 class Cleanup
	  {
		public:
		  ~Cleanup();
	  };
	  friend class Cleanup;
	   static NXConfigReader* ins;

	uint_t io_context_seconds_;
	uint_t io_context_useconds_;
	uint_t heart_beat_time_;
	uint_t log_level_;
	std::string log_path_;
	uint_t dial_timeout_;
	uint_t answer_timeout_;
	uint_t sst_timeout_;
	uint_t default_timeout_;
};


#endif
