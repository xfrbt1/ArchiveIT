std::string bytes_to_str(uintmax_t n)
{

// KB - kilobytes - 1024
// MB - megabytes - 1024 * 1024
// GB - gigabytes - 1024 * 1024 * 1024
// TB - terabytes - 1024 * 1024 * 1024 * 1024

//  if n < KB:
//      return str(n) + "bytes"
//  elif n > KB and n < MB:
//      return str(n / KB) + "KB"
//  elif n > MB and n < GB:
//      return str(n / MB) + "MB"
//  elif n > GB and n < TB:
//      return str(n / GB) + "GB"
//  elif n > TB:
//      return str(n / TB) + "TB"

    const uintmax_t KB = 1024;
    const uintmax_t MB = KB * 1024;
    const uintmax_t GB = MB * 1024;
    const uintmax_t TB = GB * 1024;

    std::string result;


    if (n < KB)
    {
        result = std::to_string(n) + " bytes";
    }
    else if (n < MB)
    {
        double size = static_cast<double>(n) / KB;
        result = std::to_string(round(size * 100) / 100) + " KB";
    }
    else if (n < GB)
    {
        double size = static_cast<double>(n) / MB;
        result = std::to_string(round(size * 100) / 100) + " MB";
    }
    else if (n < TB)
    {
        double size = static_cast<double>(n) / GB;
        result = std::to_string(round(size * 100) / 100) + " GB";
    }
    else
    {
        double size = static_cast<double>(n) / TB;
        result = std::to_string(round(size * 100) / 100) + " TB";
    }

    return result;
}