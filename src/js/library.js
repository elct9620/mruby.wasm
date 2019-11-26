mergeInto(LibraryManager.library, {
  extra: function(_unknown, mrb, self) {
    console.log(arguments);
    const inspect = ccall(
      'inspect',
      'string',
      ['number', 'number'],
      [mrb, self]
    );
    console.log(inspect)
    return self;
  },
  get_attr: function(ptr, namePtr) {
    const name = UTF8ToString(namePtr);
    const ret = ObjectSpace.get_attr(ptr, name);
    const bytes = lengthBytesUTF8(ret) + 1;
    const heap = _malloc(bytes);
    stringToUTF8(ret, heap, bytes);
    return heap;
  }
});
