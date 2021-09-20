#pragma once
#include <stratega/Representation/Vector2.h>
#include <vector>

namespace SGA {
template < typename Type >
class Grid2D {
  private:
   size_t width;
   size_t height;
   typedef typename std::vector< Type >::reference reference;
   typedef typename std::vector< Type >::const_reference const_reference;

   std::vector< Type > grid;

  public:
   template < typename InputIterator >
   Grid2D(size_t width_, InputIterator begin, InputIterator end)
       : width(width_), height(0), grid(begin, end)
   {
      if(grid.size() % width_ != 0) {
         throw std::runtime_error("Received a amount of values that is not a multiple of width.");
      }

      height = grid.size() / width_;
   }

   Grid2D(size_t width_, size_t height_, Type defaultValue = Type())
       : width(width_), height(height_), grid(width_ * height_, defaultValue)
   {
   }

   reference operator[](const Vector2i& pos) { return grid[pos.y * width + pos.x]; }
   const_reference operator[](const Vector2i& pos) const { return grid[pos.y * width + pos.x]; }

   reference get(int x, int y) { return grid[y * width + x]; }
   const_reference get(int x, int y) const { return grid[y * width + x]; }

   [[nodiscard]] size_t getWidth() const { return width; }
   [[nodiscard]] size_t getHeight() const { return height; }

   [[nodiscard]] bool isInBounds(const Vector2i& pos) const
   {
      return pos.x >= 0 && pos.x < width && pos.y >= 0 && pos.y < height;
   };
   [[nodiscard]] bool isInBounds(int x, int y) const { return isInBounds({x, y}); };

   /// <summary>
   /// Visits all positions from start to end using Bresenhams's line algorithm.
   /// </summary>
   /// <param name="start">The start position of the ray</param>
   /// <param name="end">The end position of the ray. This position can be outside of the map
   /// boundaries.</param> <param name="c">Callback triggered for every position visited. Receives
   /// as input a Vector2i and has to return a boolean to indicating if the ray should stop.</param>
   template < typename Callback >
   void bresenhamRay(const Vector2i& start, const Vector2i& end, Callback c)
   {
      auto x0 = start.x, y0 = start.y;
      auto x1 = end.x, y1 = end.y;
      auto dx = abs(x1 - x0);
      auto sx = x0 < x1 ? 1 : -1;
      auto dy = -abs(y1 - y0);
      auto sy = y0 < y1 ? 1 : -1;
      auto err = dx + dy;

      while(true) {
         if(! isInBounds(x0, y0))
            break;
         if(c(Vector2i(x0, y0)))
            break;
         if(x0 == x1 && y0 == y1)
            break;

         // Compute next position
         auto e2 = 2 * err;
         if(e2 >= dy) {
            err += dy;
            x0 += sx;
         }
         if(e2 <= dx) {
            err += dx;
            y0 += sy;
         }
      }
   }
};
}  // namespace SGA