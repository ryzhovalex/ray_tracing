-- premake5.lua
workspace "ray_tracing"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "ray_tracing"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
include "Walnut/WalnutExternal.lua"

include "ray_tracing"