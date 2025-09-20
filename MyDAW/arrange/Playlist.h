#pragma once
#include <vector>
struct Clip{ int id; long long start; long long len; int track; };
struct Playlist{ std::vector<Clip> clips; };
