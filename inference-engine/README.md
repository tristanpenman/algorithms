# Inference Engine

A small Ruby implementation of an inference engine, using three algorithms:

* Backward Chaining (BC)
* Forward Chaining (FC)
* Truth Table Checking (TT)

## IEngine

A small test program has been included, which can be started like so:

    ./bin/iengine <ALG> [path]

where `<ALG>` is one of `BC`, `FC` or `TT`, as noted above. `[path]` is an optional input file. Default behaviour is to read from STDIN.

Alternatively, you can run the test program via Rake using Bundler:

    bundle exec rake iengine:<alg>

Or, to read from a specific file:

    bundle exec rake iengine:<alg>[<path>]

Note that the algorithm/task must be in lowercase, e.g:

    bundle exec rake iengine:bc

### Commands

The program reads from STDIN, and has two modes. Entering `TELL` will switch the program into Tell mode, where it will accept any number of facts in Horn-form, separated by semi-colons or newlines. E.g:

    TELL
    p2 => p3; p3 => p1; c => e; b&e => f
    f&g => h; p1 => d; p1&p3 => c; a; b; p2;

These facts will be incorporated into the knowledge base. Note that redundant whitespace is ignored.

Entering `ASK` will switch the program into Ask mode, where the knowledge base can be queried:

    ASK
    d

The response is of the form YES or NO, depending on whether the query `q` follows from the facts in the knowledge base. All output is written to STDOUT.
