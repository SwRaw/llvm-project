// RUN: %clang_cc1 %s -fblocks -fsyntax-only -Wcast-function-type -verify
// RUN: %clang_cc1 %s -fblocks -fsyntax-only -Wcast-function-type-strict -verify

int x(long);

typedef int (f1)(long);
typedef int (f2)(void*);
typedef int (f3)(...);
typedef void (f4)(...);
typedef void (f5)(void);
typedef int (f6)(long, int);
typedef int (f7)(long,...);
typedef int (&f8)(long, int);

f1 *a;
f2 *b;
f3 *c;
f4 *d;
f5 *e;
f6 *f;
f7 *g;

struct S
{
  void foo (int*);
  void bar (int);
};

typedef void (S::*mf)(int);

enum E : long;
int efunc(E);

// Produce the underlying `long` type implicitly.
enum E2 { big = __LONG_MAX__ };
int e2func(E2);

void foo() {
  a = (f1 *)x;
  a = (f1 *)efunc; // expected-warning {{cast from 'int (*)(E)' to 'f1 *' (aka 'int (*)(long)') converts to incompatible function type}}
  a = (f1 *)e2func; // expected-warning {{cast from 'int (*)(E2)' to 'f1 *' (aka 'int (*)(long)') converts to incompatible function type}}
  b = (f2 *)x; // expected-warning {{cast from 'int (*)(long)' to 'f2 *' (aka 'int (*)(void *)') converts to incompatible function type}}
  b = reinterpret_cast<f2 *>(x); // expected-warning {{cast from 'int (*)(long)' to 'f2 *' (aka 'int (*)(void *)') converts to incompatible function type}}
  c = (f3 *)x; // expected-warning {{cast from 'int (*)(long)' to 'f3 *' (aka 'int (*)(...)') converts to incompatible function type}}
  d = (f4 *)x; // expected-warning {{cast from 'int (*)(long)' to 'f4 *' (aka 'void (*)(...)') converts to incompatible function type}}
  e = (f5 *)x; // expected-warning {{cast from 'int (*)(long)' to 'f5 *' (aka 'void (*)()') converts to incompatible function type}}
  f = (f6 *)x; // expected-warning {{cast from 'int (*)(long)' to 'f6 *' (aka 'int (*)(long, int)') converts to incompatible function type}}
  g = (f7 *)x; // expected-warning {{cast from 'int (*)(long)' to 'f7 *' (aka 'int (*)(long, ...)') converts to incompatible function type}}

  mf p1 = (mf)&S::foo; // expected-warning {{cast from 'void (S::*)(int *)' to 'mf' (aka 'void (S::*)(int)') converts to incompatible function type}}

  f8 f2 = (f8)x; // expected-warning {{cast from 'int (long)' to 'f8' (aka 'int (&)(long, int)') converts to incompatible function type}}
  (void)f2;

  int (^y)(long);
  f = (f6 *)y; // expected-warning {{cast from 'int (^)(long)' to 'f6 *' (aka 'int (*)(long, int)') converts to incompatible function type}}
}
