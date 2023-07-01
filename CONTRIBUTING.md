# How to contribute

In general the language for communication and documentation in this project is US English.

## Commit messages

Type a short, meaningful commit message that describes the change you made to the file.

Please consider [the recommendation of the Git community for commit messages](https://git-scm.com/docs/git-commit/2.38.0#_discussion):

> Begin the commit message with a single short (less than 50 character) line summarizing the change, followed by a blank line and then a more thorough description. The text up to the first blank line in a commit message is treated as the commit title, and that title is used throughout Git.

## Pull requests

Use self-reviews to speed up the review process.
This includes:

- Review the content for technical accuracy.
- If there are any failing checks in your PR, troubleshoot them until they're all passing.

## Document your code

Please document your code in Doxygen compatible syntax.

## C/C++ source code

Please format your changes using [ClangFormat](https://clang.llvm.org/docs/ClangFormat.html).
A style configuration file is given: [`.clang-format`](.clang-format)

Although the standalone tool `clang-format` may be used, we recommend to integrate the formatting using a [plugin](https://firefox-source-docs.mozilla.org/code-quality/coding-style/format_cpp_code_with_clang-format.html#editor-plugins) for the editor of your choice.

## Doxygen configuration

If you change [`Doxyfile`](Doxyfile), please do a ['cleanup'](https://stackoverflow.com/questions/71157463/what-part-of-the-doxygen-configuration-doxyfile-is-recommended-for-version-con#comment125783393_71157463) by using the output of `doxygen -x`.
For example

    doxygen -x > Doxyfile.tmp && mv Doxyfile.tmp Doxyfile

## PlatformIO configuration

If you change [`platformio.ini`](platformio.ini) using [PlatformIO Home](https://docs.platformio.org/en/latest/home/index.html#platformio-home), please note that [comments may be deleted unsolicitedly](https://github.com/platformio/platformio-core/issues/3862) by that tool.
In general we want to preserve useful comments in the project configuration file.
