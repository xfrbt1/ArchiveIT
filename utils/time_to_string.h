std::string time_to_string(const std::chrono::system_clock::time_point &tp)
{
    std::time_t time = std::chrono::system_clock::to_time_t(tp);
    std::string timeStr = std::ctime(&time);
    return timeStr;
}