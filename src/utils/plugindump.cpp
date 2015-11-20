#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <glob.h>
#include <dlfcn.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>
#include <algorithm>
#include <glob.h>
#include <dlfcn.h>
#include <ut/utils.h>

void
doit (int argc, char **argv, std::string path="")
{
    void *                      handle;
    char *                      error;

    glob_t                      globbuf;
    std::string                 pattern;

    std::vector<std::string>    pathList;
    std::vector<std::string>    pluginList;

    // start with passed in path
    if (path.length())
        pathList.push_back(path);

    // then PLUGIN_PATH
    {
        auto tokens = Ut::splitString(PLUGIN_PATH, ";");
        std::copy(std::begin(tokens), std::end(tokens), std::back_inserter(pathList));
    }

    // and finally append path(s) in SG_PLUGIN_PATH envvar
    if (getenv("SG_PLUGIN_PATH"))
    {
        auto tokens = Ut::splitString(getenv("SG_PLUGIN_PATH"), ";");
        std::copy(std::begin(tokens), std::end(tokens), std::back_inserter(pathList));
    }

    // build list of *.so files in pathlist
    for (const auto &it : pathList)
    {
        // glob for plugins
        pattern = it + "/*.so";
        globbuf.gl_offs = 0;
        // TODO : check for error on glob() call
        glob(pattern.c_str(), GLOB_ERR,0, &globbuf);

        for(uint i=0; i < globbuf.gl_pathc; ++i)
            pluginList.push_back(globbuf.gl_pathv[i]);

        globfree(&globbuf);
    }

    typedef std::string (*Info)();

    // attempt to load found plugins
    for (const auto &it : pluginList)
    {
        // try to open each shared-lib
        handle = dlopen(it.c_str(), RTLD_LAZY | RTLD_LOCAL);

        if(!handle)
        {
           std::cerr << "Cannot dlopen: " << dlerror() << std::endl;
           continue;
        }

        // load the symbols
        dlerror();
        dlsym (handle, "registerNode");
        if ((error = dlerror()) != NULL)
        {
            std::cerr << "Cannot load symbols: " << error << std::endl;
            dlclose(handle);
            continue;
        }
        dlerror();
        auto name = (Info) dlsym (handle, "name");
        if ((error = dlerror()) != NULL)
        {
            std::cerr << "Cannot load symbols: " << error << std::endl;
            dlclose(handle);
            continue;
        }
        dlerror();
        auto type = (Info) dlsym (handle, "type");
        if ((error = dlerror()) != NULL)
        {
            std::cerr << "Cannot load symbols: " << error << std::endl;
            continue;
        }

        dlclose(handle);

        std::cout << "plugin " << it << std::endl;
        std::cout << "  name   = " << name() << std::endl;
        std::cout << "  type   = " << type() << std::endl;
        std::cout << std::endl;
    }
}

int
main (int argc, char **argv)
{
    try
    {
        doit(argc, argv);
    }

    catch (std::ios::failure &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 1;
}
