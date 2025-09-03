#pragma once

#include <iostream>
#include <fstream>
#include "../Model/MapObjManager.h"
#include "../Model/EnemyInfoManager.h"
#include "../Model/HeaderOnlyCsv.hpp"
#include <tchar.h>

void WriteBinaryFromMapObj(const std::wstring& csvFile);

void WriteBinaryFromEnemy(const std::wstring& csvFile);

