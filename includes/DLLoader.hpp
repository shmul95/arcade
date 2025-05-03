/*
** EPITECH PROJECT, 2025
** arcade
** File description:
** DLLoader
*/

#pragma once

#include <iostream>
#include <fstream>
#include <dlfcn.h>
#include <memory>

#include "IGraphics.hpp"
#include "IGame.hpp"

void handle_error(const char *msg);

namespace Arcade
{
    template <typename T>
    class DLLoader
    {
    private:
        std::string _name;
        void *_handler;

        void open_lib(std::ofstream &logFile)
        {
            _handler = dlopen(this->_name.c_str(), RTLD_NOW);
            if (!_handler) {
                logFile << "/!\\ [" << this->_name.c_str() << "] Cannot open library: " << dlerror() << std::endl;
                handle_error("Cannot open library");
            }
            dlerror();
        }

        T *get_instance(std::ofstream &logFile, std::string sym) const
        {
            T *(*createInstance)(void) = (T *(*)(void))dlsym(_handler, ("create" + sym).c_str());
            if (!createInstance) {
                logFile << "/!\\ [" << this->_name.c_str() << "] Cannot load constructor: " << dlerror() << std::endl;
                handle_error("Cannot load constructor");
            }
            return createInstance();
        }

        T *get_graphics(std::ofstream &logFile) const { return get_instance(logFile, "Graphics"); }
        T *get_games(std::ofstream &logFile) const { return get_instance(logFile, "Game"); }

    public:
        DLLoader(const std::string &libName)
            : _name(libName), _handler(nullptr)
        {
            std::ofstream logFile("DLLoader.log", std::ios::app);

            logFile << "[" << this->_name.c_str() << "] Creating..." << std::endl;            
            open_lib(logFile);
            logFile << "[" << this->_name.c_str() << "] Created !" << std::endl;
            logFile.close();
        };

        void close(void)
        {
            std::ofstream logFile("DLLoader.log", std::ios::app);
            logFile << "[" << this->_name.c_str() << "] Destroy" << std::endl;
            if (_handler) {
                dlclose(_handler);
                _handler = nullptr;
            }
            logFile.close();
        }

        ~DLLoader() { close(); }

        T *getInstance() const {
            T *instance = nullptr;
            std::ofstream logFile("DLLoader.log", std::ios::app);

            if constexpr (std::is_same<IGraphics, T>::value) {
                instance = get_graphics(logFile);
                logFile << "[" << this->_name.c_str() << "] Graphics instance created" << std::endl;
                logFile.close();
                return instance;
            } else if constexpr (std::is_same<IGame, T>::value) {
                instance = get_games(logFile);
                logFile << "[" << this->_name.c_str() << "] Game instance created" << std::endl;
                logFile.close();
                return instance;
            } else
                logFile << "/!\\ DLLoader can only load IGame or IGraphics" << std::endl;
            return nullptr;
        }

        std::string getName() const {
            return _name;
        }
    };
} // namespace Arcade
