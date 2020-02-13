#include "NXConfigReader.h"
#include "CConf.h"

const char* const NXConfigReader::kHeartBeat = "HEARTBEAT_TIMEOUT";
const char* const NXConfigReader::kIoContextSecond = "IOCONTEXT_SEC";
const char* const NXConfigReader::kIoContextMicroSecond = "IOCONTEXT_USEC";
const char* const NXConfigReader::kLogLevel = "LOGLEVEL";
const char* const NXConfigReader::kLogPath = "LOGPATH";
const char* const NXConfigReader::kDialTimeOut = "DIAL_TIMEOUT";
const char* const NXConfigReader::kSSTTimeOut = "SST_TIMEOUT";
const char* const NXConfigReader::kAnswerTimeOut = "ANSWER_TIMEOUT";


NXConfigReader* NXConfigReader::ins = nullptr;
NXConfigReader& NXConfigReader::instance()
{
  static Cleanup up;
  if(ins == nullptr) 
    ins = new NXConfigReader();
  return *ins;
}

NXConfigReader::NXConfigReader()
{
}
	
NXConfigReader::~NXConfigReader()
{

}

NXConfigReader::Cleanup::~Cleanup()
{
  delete NXConfigReader::ins;
  NXConfigReader::ins = nullptr;
}

void NXConfigReader::set_io_context_seconds(const uint_t s)
{
	io_context_seconds_ = s;
}
void NXConfigReader::set_io_context_useconds(const uint_t us)
{
	io_context_useconds_ = us;
}
void NXConfigReader::set_heart_beat_time(const uint_t t)
{
	heart_beat_time_ = t;
}
void NXConfigReader::set_log_level(const uint_t lv)
{
	log_level_ = lv;
}
void NXConfigReader::set_log_path(const char* path)
{
	log_path_ = path;
}
void NXConfigReader::set_dial_timeout(const uint_t t)
{
	dial_timeout_ = t;
}
void NXConfigReader::set_answer_timeout(const uint_t t)
{
	answer_timeout_ = t;
}
void NXConfigReader::set_sst_timeout(const uint_t t)
{
	sst_timeout_ = t;
}
	
uint_t NXConfigReader::log_level() const
{
	return log_level_;
}
std::string NXConfigReader::log_path() const
{
	return log_path_;
}
uint_t NXConfigReader::hearbeat_time() const
{
	return heart_beat_time_;
}
uint_t NXConfigReader::io_context_seconds() const
{
	return io_context_seconds_;
}
uint_t NXConfigReader::io_context_useconds() const
{
	return io_context_useconds_;
}
uint_t NXConfigReader::dial_timeout() const
{
	return dial_timeout_;
}
uint_t NXConfigReader::answer_timeout() const
{
	return answer_timeout_;
}
uint_t NXConfigReader::sst_timeout() const
{
	return sst_timeout_;
}

void NXConfigReader::LoadConfigure(const char* const file_name)
{
	CConf config(file_name);
	int_t temp_value = 0;
	std::string temp_string = "";

	temp_value = config.getINT(kIoContextSecond);

	if(temp_value > 0) io_context_seconds_ = temp_value;
	temp_value = config.getINT(kIoContextMicroSecond);
	if(temp_value > 0) io_context_useconds_ = temp_value;
	temp_value = config.getINT(kHeartBeat);
	if(temp_value > 0 ) heart_beat_time_ = temp_value;
	temp_value = config.getINT(kDialTimeOut);
	if(temp_value > 0 ) dial_timeout_ = temp_value;
	temp_value = config.getINT(kAnswerTimeOut);
	if(temp_value > 0 ) answer_timeout_ = temp_value;
	temp_value = config.getINT(kSSTTimeOut);
	if(temp_value > 0 ) sst_timeout_ = temp_value;
	temp_value = config.getINT(kLogLevel);
	if(temp_value > 0 ) log_level_= temp_value;
	temp_string = config.getString(kLogPath);
	if(temp_string != "") log_path_ = temp_string;
}
