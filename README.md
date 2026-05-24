This repo reproduces an issue when using toolchains_llvm 1.6.0 with a
p> k
remote_cache. Bazel rebuilds unnecesarily after a restarting the bazel server
when it could use the action cache.

To reproduce the issue: build, then shutdown, then rebuild. Bazel will rebuild targets that require the llvm toolchain.

````
$ bazel clean

INFO: Invocation ID: 5b3cfb00-65a8-49fc-a5eb-a87f55c113b1
INFO: Starting clean (this may take a while). Use --async if the clean takes more than several minutes.

bazel build ...

INFO: Invocation ID: c5a7509f-59a5-46d0-a0f5-3496dce92a19
INFO: Analyzed 4 targets (114 packages loaded, 6144 targets configured).
WARNING: Remote Cache: Connection refused: /127.0.0.1:404
WARNING: Remote Cache: 4 errors during bulk transfer:
io.netty.channel.AbstractChannel$AnnotatedConnectException: Connection refused: /127.0.0.1:404
io.netty.channel.AbstractChannel$AnnotatedConnectException: Connection refused: /127.0.0.1:404
io.netty.channel.AbstractChannel$AnnotatedConnectException: Connection refused: /127.0.0.1:404
io.netty.channel.AbstractChannel$AnnotatedConnectException: Connection refused: /127.0.0.1:404
WARNING: Remote Cache: 3 errors during bulk transfer:
io.netty.channel.AbstractChannel$AnnotatedConnectException: Connection refused: /127.0.0.1:404
io.netty.channel.AbstractChannel$AnnotatedConnectException: Connection refused: /127.0.0.1:404
io.netty.channel.AbstractChannel$AnnotatedConnectException: Connection refused: /127.0.0.1:404
INFO: Found 4 targets...
INFO: Elapsed time: 22.739s, Critical Path: 15.95s
INFO: 335 processes: 283 internal, 52 linux-sandbox.
INFO: Build completed successfully, 335 total actions

$ bazel shutdown

$ bazel build ...

Starting local Bazel server (8.5.1) and connecting to it...
INFO: Invocation ID: 15200f13-2c1f-4768-9cb1-5413485de04c
INFO: Analyzed 4 targets (114 packages loaded, 6144 targets configured).
WARNING: Remote Cache: Connection refused: /127.0.0.1:404
WARNING: Remote Cache: 4 errors during bulk transfer:
io.netty.channel.AbstractChannel$AnnotatedConnectException: Connection refused: /127.0.0.1:404
io.netty.channel.AbstractChannel$AnnotatedConnectException: Connection refused: /127.0.0.1:404
io.netty.channel.AbstractChannel$AnnotatedConnectException: Connection refused: /127.0.0.1:404
io.netty.channel.AbstractChannel$AnnotatedConnectException: Connection refused: /127.0.0.1:404
INFO: Found 4 targets...
INFO: Elapsed time: 27.915s, Critical Path: 14.36s
INFO: 50 processes: 287 action cache hit, 1 internal, 49 linux-sandbox.
INFO: Build completed successfully, 50 total actions
````

Now, if you remove `build --remote_cache=http://127.0.0.1:404` from the
.bazelrc (or just remove the .bazelrc), a shutdown will not cause a rebuild.
All targets are found in the action cache.

````
$ rm .bazelrc
$ bazel shutdown
$ bazel build ...

Starting local Bazel server (8.5.1) and connecting to it...
INFO: Analyzed 4 targets (114 packages loaded, 6144 targets configured).
INFO: Found 4 targets...
INFO: Elapsed time: 3.881s, Critical Path: 0.08s
INFO: 1 process: 336 action cache hit, 1 internal.
INFO: Build completed successfully, 1 total action
````
