# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/shawn/source/repos/sand-game/out/build/x64-release/_deps/sdl2-src"
  "C:/Users/shawn/source/repos/sand-game/out/build/x64-release/_deps/sdl2-build"
  "C:/Users/shawn/source/repos/sand-game/out/build/x64-release/_deps/sdl2-subbuild/sdl2-populate-prefix"
  "C:/Users/shawn/source/repos/sand-game/out/build/x64-release/_deps/sdl2-subbuild/sdl2-populate-prefix/tmp"
  "C:/Users/shawn/source/repos/sand-game/out/build/x64-release/_deps/sdl2-subbuild/sdl2-populate-prefix/src/sdl2-populate-stamp"
  "C:/Users/shawn/source/repos/sand-game/out/build/x64-release/_deps/sdl2-subbuild/sdl2-populate-prefix/src"
  "C:/Users/shawn/source/repos/sand-game/out/build/x64-release/_deps/sdl2-subbuild/sdl2-populate-prefix/src/sdl2-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/shawn/source/repos/sand-game/out/build/x64-release/_deps/sdl2-subbuild/sdl2-populate-prefix/src/sdl2-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/shawn/source/repos/sand-game/out/build/x64-release/_deps/sdl2-subbuild/sdl2-populate-prefix/src/sdl2-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
