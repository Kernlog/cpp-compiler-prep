# Cpp-Compiler-prep

From-scratch compiler in C++ for a custom language. Full pipeline, minimal dependencies.

## Overview

Building a complete compiler from scratch. The goal is to actually understand each stage rather than just wrapping LLVM.

Pipeline: source -> tokens -> AST -> type-checked AST -> IR -> optimized IR -> bytecode -> VM execution

## Components

**Lexer** - Hand-rolled tokenizer. No regex. Track line/col for errors.

**Parser** - Recursive descent, Pratt for expressions. Outputs an AST.

**Semantic Analysis** - Type checking, name resolution, scope management, symbol tables.

**IR** - Probably SSA. Makes optimization cleaner even if phi nodes are annoying.

**Optimization** - Constant folding, dead code elimination. Keep it simple initially.

**Codegen** - Emit bytecode for the VM. Stack-based first, easier to target.

**VM** - Bytecode interpreter. Switch dispatch to start, threaded later if needed.

**Runtime** - GC (mark-sweep), builtins, memory management.

## Project Structure

```
src/
  lexer/
  parser/
  ast/
  sema/
  ir/
  opt/
  codegen/
  vm/
  runtime/
  common/
```

## Build

```
mkdir build && cd build
cmake ..
make
```

## Implementation Order

1. Get lexer working with basic tokens
2. Parser that builds a minimal AST (just expressions first)
3. Tree-walk interpreter to validate the AST works
4. Add statements, control flow, functions to lexer/parser
5. Semantic analysis pass
6. IR generation
7. Basic optimization passes
8. Bytecode emission
9. VM implementation
10. Runtime/GC

Each stage should be testable independently before moving on.

## Language Design (TBD)

Still deciding on:
- Memory model - GC vs manual vs RAII
- Type system strictness
- Closures/first-class functions
- Generics

Leaning C-like syntax with some modern conveniences. Will document decisions as they're made.

## Current Status

Starting from scratch.
