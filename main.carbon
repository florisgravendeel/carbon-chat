package Chatserver api;
import Math;

fn PrintTotalArea(dx: i32, dy: i32) {
  var area: auto = 0;
  
  area = Math.Pi * dx * dx;
  
  Print("Total area: {0}", area);
}

fn Main() -> i32 {
  // A dynamically sized array, like `std::vector`.
  
  // Implicitly constructs `Slice` from `Array`.
  PrintTotalArea(1, 2);
  return 0;
}