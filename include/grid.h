// Implements a 2D grid
// Created by Kevin Gori on 01/03/2021.
//

#pragma once
#include <algorithm>
#include <cassert>
#include <iostream>
#include <map>
#include <utility>
#include <vector>
#include <SFML/Window/Event.hpp>

#undef CAVEGEN
#define GAMEOFLIFE

using Coord = std::pair<unsigned int, unsigned int>;
using CoordVec = std::vector<Coord>;

class Grid {
public:
    Grid(unsigned int height, unsigned int width)
            : height_(height), width_(width), paused_(true) {

        data_.resize(width * height, 0);
        counts_.resize(width * height, 0);

        build_map();
    }

    bool isPaused() const {
        return paused_;
    }

    void handleInputs(const std::vector<sf::Event>& events, float cellWidth, float cellHeight) {
        for (auto &event : events) {
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    unsigned int x = floor(static_cast<float>(event.mouseButton.x) / cellWidth);
                    unsigned int y = floor(static_cast<float>(event.mouseButton.y) / cellHeight);
                    toggle(x, y);
                }
                else if (event.mouseButton.button == sf::Mouse::Right) {
                    paused_ = !paused_;
                }
            }
        }
    }

    friend void swap(Grid &a, Grid &b) {
        using std::swap;
        swap(a.height_, b.height_);
        swap(a.width_, b.width_);
        swap(a.data_, b.data_);
        swap(a.counts_, b.counts_);
        swap(a.neighbours_, b.neighbours_);
    }

    void set(unsigned int x, unsigned int y) {
        if (data_[xy_to_index(x, y)] == 0) {
            data_[xy_to_index(x, y)] = 1;
            for (auto ix : neighbours_[xy_to_index(x, y)]) {
                counts_[ix] += 1;
            }
        }
    }

    void unset(unsigned int x, unsigned int y) {
        if (data_[xy_to_index(x, y)] == 1) {
            data_[xy_to_index(x, y)] = 0;
            for (auto ix : neighbours_[xy_to_index(x, y)]) {
                counts_[ix] -= 1;
            }
        }
    }

    /// TODO: Somewhere I mixed up the meaning of x and y - fix this
    void toggle(unsigned int y, unsigned int x) {
        data_[xy_to_index(x, y)] == 0 ? set(x, y) : unset(x, y);
    }

    void regenerate_neighbour_counts() {
        std::fill(counts_.begin(), counts_.end(), 0);
        for (int x = 0; x < height_; ++x) {
            for (int y = 0; y < width_; ++y) {
                auto value = get_value(x, y);
                for (auto ix : neighbours_[xy_to_index(x, y)]) {
                    counts_[ix] += value;
                }
            }
        }
    }

    int get_value(unsigned int x, unsigned int y) const {
        return data_[xy_to_index(x, y)];
    }

    int get_neighbour_count(unsigned int x, unsigned int y) const {
        return counts_[xy_to_index(x, y)];
    }

    std::vector<int> get_values() {
        return data_;
    }

    void print_values() const {
        for (int x = 0; x < height_; ++x) {
            for (int y = 0; y < width_; ++y) {
                std::cout << (get_value(x, y) == 1 ? "\u2593" : "\u2591");
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    void print_counts() const {
        for (int x = 0; x < height_; ++x) {
            for (int y = 0; y < width_; ++y) {
                std::cout << get_neighbour_count(x, y);
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    std::vector<std::pair<unsigned int, unsigned int>> to_switch_on() {

        std::vector<std::pair<unsigned int, unsigned int>> output;

        for (int x = 0; x < height_; ++x) {
            for (int y = 0; y < width_; ++y) {
#if defined(CAVEGEN)
                if (get_value(x, y) == 0 && get_neighbour_count(x, y) > 4) { // cave generation
#elif defined(GAMEOFLIFE)
                if (get_value(x, y) == 0 && get_neighbour_count(x, y) == 3) { // game of life
#else
                if (get_value(x, y) == 0 && (get_neighbour_count(x, y) == 3 || get_neighbour_count(x, y) == 6)) { // game of life - high life variation
#endif
                    output.push_back(std::make_pair(x, y));
                }
            }
        }

        return output;
    }

    std::vector<std::pair<unsigned int, unsigned int>> to_switch_off() {

        std::vector<std::pair<unsigned int, unsigned int>> output;

        for (int x = 0; x < height_; ++x) {
            for (int y = 0; y < width_; ++y) {
#ifdef CAVEGEN
                if (get_value(x, y) == 1 && get_neighbour_count(x, y) < 3) { // cave generation
#else
                if (get_value(x, y) == 1 && (get_neighbour_count(x, y) < 2 || get_neighbour_count(x, y) >= 4)) { // game of life
#endif
                    output.push_back(std::make_pair(x, y));
                }
            }
        }

        return output;
    }

    unsigned int update() {
        auto on_list = to_switch_on();
        auto off_list = to_switch_off();
        for (auto [x, y] : off_list) {
            unset(x, y);
        }
        for (auto [x, y] : on_list) {
            set(x, y);
        }

        return on_list.size() + off_list.size();
    }

private:
    unsigned int width_, height_;
    bool paused_;
    std::vector<int> data_, counts_;
    std::map<unsigned int, std::vector<unsigned int>> neighbours_;

    unsigned int xy_to_index(unsigned int x, unsigned int y) const {
        assert (x < height_);
        assert (y < width_);
        return x * width_ + y;
    }

    std::vector<unsigned int> get_neighbour_indices(unsigned int x, unsigned int y) const {
        assert (x < height_);
        assert (y < width_);
        unsigned int above = x > 0 ? x - 1 : height_ - 1;
        unsigned int below = x == (height_ - 1) ? 0 : x + 1;
        unsigned int left = y > 0 ? y - 1 : width_ - 1;
        unsigned int right = y == (width_ - 1) ? 0 : y + 1;

        return std::vector<unsigned int>{
            xy_to_index(above, left),
            xy_to_index(above, y),
            xy_to_index(above, right),
            xy_to_index(x, left),
            xy_to_index(x, right),
            xy_to_index(below, left),
            xy_to_index(below, y),
            xy_to_index(below, right),
        };
    }

    void build_map() {
        for (unsigned int x = 0; x < height_; ++x) {
            for (unsigned int y = 0; y < width_; ++y) {
                unsigned int ix = xy_to_index(x, y);
                neighbours_[ix] = get_neighbour_indices(x, y);
            }
        }
    }
};
