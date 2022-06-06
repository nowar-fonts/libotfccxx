# libotfcc++ (libvf-ng)

Modern C++ OpenType library based on otfcc and ot-builder.

## Design

### Modern C++

Make use of modern C++ language core and libraries.

### Header Only

```c++
#include <otfcc.hpp>
```

### All Grammatically Public

All members are grammatically `public`. Semantically private members are prefixed with an underscore (`_`).

You can easily touch any member if necessary – there are too many corner cases in OpenType.
