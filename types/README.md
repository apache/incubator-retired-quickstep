# The Quickstep Type System

The types module is used across Quickstep and handles details of how date values
are stored and represented, how they are parsed from and printed to
human-readable text, and low-level operations on values that form the building
blocks for more complex [expressions](../expressions).

## The Type Class

Every distinct concrete type in Quickstep is represented by a single object of
a class derived from the base `quickstep::Type` class. All types have some
common properties, including the following:

  * A `TypeID` - an enum identifying the type, e.g. `kInt` for 32-bit integers,
    or `kVarChar` for variable-length strings.
  * Nullability - whether the type allows NULL values. All types have both a
    nullable and a non-nullable flavor, except for NullType, a special type that
    can ONLY store NULLs and has no non-nullable version.
  * Storage size - minimum and maximum byte length. For fixed-length types like
    basic numeric types and fixed length `CHAR(X)` strings, these lengths are
    the same. For variable-length types like `VARCHAR(X)`, they can be
    different (and the `Type` class has a method `estimateAverageByteLength()`
    that can be used to make educated guesses when allocating storage). Note
    that storage requirements really only apply to uncompressed, non-NULL
    values. The actual bytes needed to store the values in the
    [storage system](../storage) may be different if
    [compression](../compression) is used, and some storage formats might store
    NULLs differently.

Some categories of types have additional properties (e.g. `CharType` and
`VarCharType` also have a length parameter that indicates the maximum length of
string that can be stored).

### Getting a Type

Each distinct, concrete Type is represented by a single object in the entire
Quickstep process. To actually get a reference to usable `Type`, most code will
go through the `TypeFactory`. `TypeFactory` provides static methods to access
specific types by `TypeID` and other parameters. It can also deserialize a type
from its protobuf representation (a `quickstep::serialization::Type` message).
Finally, it also provides methods that can determine a `Type` that two different
types can be cast to.

### More on the `Type` Interface

In addition to methods that allow inspection of a type's properties (e.g. those
listed above), the Type class defines an interface with useful functionality
common to all types:

  * Serialization (of the type itself) - the `getProto()` method produces a
    protobuf message that can be serialized and deserialized and later passed to
    the TypeFactory to get back the same type.
  * Relationship to other types - `equals()` determines if two types are exactly
    the same, while `isCoercibleFrom()` determines if it is possible to convert
    from another type to a given type (e.g. with a `CAST`), and
    `isSafelyCoercibleFrom()` determines if such a conversion can always be done
    without loss of precision.
  * Printing to human-readable format - `printValueToString()` and
    `printValueToFile()` can print out values of a type (see `TypedValue` below)
    in human-readable format.
  * Parsing from human-readable format - Similarly, `parseValueFromString()`
    produces a `TypedValue` that is parsed from a string in human-readable
    format.
  * Making values - `makeValue()` creates a `TypedValue` from a bare pointer to
    a value's representation in storage. For nullable types, `makeNullValue()`
    makes a NULL value, and for numeric types, `makeZeroValue()` makes a zero
    of that type.
  * Coercing values - `coerceValue()` takes a value of another type and converts
    it to the given type (e.g. as part of a `CAST`).

## The TypedValue Class

An individual typed value in Quickstep is represented by an instance of the
`TypedValue` class. TypedValues can be created by methods of the `Type` class,
by operation and expression classes that operate on values, or simply by calling
one of several constructors provided in the class itself for convenience.
TypedValues have C++ value semantics (i.e. they are copyable, assignable, and
movable). A TypedValue may own its own data, or it may be a lightweight
reference to data that is stored elsewhere in memory (this can be checked with
`isReference()`, and any reference can be upgraded to own its own data copy by
calling `ensureNotReference()`).

Here are some of the things you can do with a TypedValue:

  * NULL checks - calling `isNull()` determines if the TypedValue represents a
    NULL. Several methods of TypedValue are usable only for non-NULL values, so
    it is often important to check this first if in doubt.
  * Access to underlying data - `getDataPtr()` returns an untyped `void*`
    pointer to the underlying data, and `getDataSize()` returns the size of the
    underlying data in bytes. Depending on the type of the value, the templated
    method `getLiteral()` can be used to get the underlying data as a literal
    scalar, or `getAsciiStringLength()` can be used to get the string length of
    a `CHAR(X)` or `VARCHAR(X)` without counting null-terminators.
  * Hashing - `getHash()` returns a hash of the value, which is suitable for
    use in the HashTables of the [storage system](../storage), or in generic
    hash-based C++ containers. `fastEqualCheck()` is provided to quickly check
    whether two TypedValues of the same type (e.g. in the same hash table) are
    actually equal.
  * Serialization/Deserialization - `getProto()` serializes a TypedValue to a
    `serialization::TypedValue` protobuf. The static method `ProtoIsValid()`
    checks whether a serialized TypedValue is valid, and
    `ReconstructFromProto()` rebuilds a TypedValue from its serialized form.
