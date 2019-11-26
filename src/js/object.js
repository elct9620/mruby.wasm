this['ObjectSpace'] = {
  handle: [],
  register(object) {
    const ptr = this.handle.length;
    this.handle.push(object)
    return ptr;
  },
  get_attr(ptr, prototypeName) {
    if (!this.handle[ptr]) {
      return null;
    }
    console.log('Try get: ', this.handle[ptr][prototypeName])

    return this.handle[ptr][prototypeName];
  }
}
