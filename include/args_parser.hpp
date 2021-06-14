#pragma once


struct Configuration
{   
    bool printUsage = false;
    bool silent = false;
    bool measurePerformance = false;
    bool interactive = false;
};

Configuration ParseCmdArgs(int argc, char **argv, int &argc_flags_offset_out);
