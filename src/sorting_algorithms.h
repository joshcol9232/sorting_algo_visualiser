#pragma once

#include <memory>
#include "sort_array.h"

void bubble_sort(std::shared_ptr<SortArray> arr);

void bogo_sort(std::shared_ptr<SortArray> arr);

std::shared_ptr<SortArray> quick_sort(std::shared_ptr<SortArray> arr);

