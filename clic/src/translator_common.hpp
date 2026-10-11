#ifndef __SRC_TRANSLATOR_COMMON_HPP
#define __SRC_TRANSLATOR_COMMON_HPP

// Private text-processing helpers shared by the OpenCL -> CUDA / Metal translators (not installed).
// The translators work by text substitution: every helper here operates on raw source text.

#include <cctype>
#include <optional>
#include <string>
#include <vector>

namespace cle::detail
{

[[nodiscard]] inline auto
isWordChar(char c) -> bool
{
  return std::isalnum(static_cast<unsigned char>(c)) || c == '_';
}

/// Replace every occurrence of `from`.
inline auto
replaceAll(std::string & str, const std::string & from, const std::string & to) -> void
{
  if (from.empty())
  {
    return;
  }
  for (size_t pos = str.find(from); pos != std::string::npos; pos = str.find(from, pos))
  {
    str.replace(pos, from.size(), to);
    pos += to.size();
  }
}

/// Replace the occurrences of `from` that satisfy the requested word boundaries (before and/or after it).
inline auto
replaceBounded(std::string & str, const std::string & from, const std::string & to, bool boundary_before, bool boundary_after) -> void
{
  if (from.empty())
  {
    return;
  }
  size_t pos = 0;
  while ((pos = str.find(from, pos)) != std::string::npos)
  {
    const bool word_before = boundary_before && pos > 0 && isWordChar(str[pos - 1]);
    const bool word_after = boundary_after && pos + from.size() < str.size() && isWordChar(str[pos + from.size()]);
    if (word_before || word_after)
    {
      pos += from.size();
      continue;
    }
    str.replace(pos, from.size(), to);
    pos += to.size();
  }
}

/// Replace `from` when it is a whole word.
inline auto
replaceWord(std::string & str, const std::string & from, const std::string & to) -> void
{
  replaceBounded(str, from, to, true, true);
}

/// Replace `from` when it is not followed by a word character (e.g. "__global" but not "__global__").
inline auto
replaceSuffix(std::string & str, const std::string & from, const std::string & to) -> void
{
  replaceBounded(str, from, to, false, true);
}

/// Replace `from` when it is not preceded by a word character (e.g. "as_int(" but not "has_int(").
inline auto
replacePrefix(std::string & str, const std::string & from, const std::string & to) -> void
{
  replaceBounded(str, from, to, true, false);
}

/// Collapse runs of spaces into one, and runs of 3+ newlines into 2.
inline auto
collapseWhitespace(std::string & code) -> void
{
  size_t write = 0;
  size_t spaces = 0;
  size_t newlines = 0;
  for (const char c : code)
  {
    spaces = (c == ' ') ? spaces + 1 : 0;
    newlines = (c == '\n') ? newlines + 1 : 0;
    if (spaces <= 1 && newlines <= 2)
    {
      code[write++] = c;
    }
  }
  code.resize(write);
}

[[nodiscard]] inline auto
trimWhitespace(const std::string & s) -> std::string
{
  size_t b = 0;
  while (b < s.size() && std::isspace(static_cast<unsigned char>(s[b])))
  {
    ++b;
  }
  size_t e = s.size();
  while (e > b && std::isspace(static_cast<unsigned char>(s[e - 1])))
  {
    --e;
  }
  return s.substr(b, e - b);
}

/// Position of the bracket closing the one at `open` (`(`/`)` or `{`/`}`), or npos.
[[nodiscard]] inline auto
findClosing(const std::string & code, size_t open, char open_char = '(', char close_char = ')') -> size_t
{
  int depth = 0;
  for (size_t i = open; i < code.size(); ++i)
  {
    if (code[i] == open_char)
    {
      ++depth;
    }
    else if (code[i] == close_char && --depth == 0)
    {
      return i;
    }
  }
  return std::string::npos;
}

/// Positions of the commas in [begin, end) that are not nested in parentheses.
[[nodiscard]] inline auto
topLevelCommas(const std::string & code, size_t begin, size_t end) -> std::vector<size_t>
{
  std::vector<size_t> commas;
  int                 depth = 0;
  for (size_t i = begin; i < end; ++i)
  {
    if (code[i] == '(')
    {
      ++depth;
    }
    else if (code[i] == ')')
    {
      --depth;
    }
    else if (code[i] == ',' && depth == 0)
    {
      commas.push_back(i);
    }
  }
  return commas;
}

/// A call `name(...)` found in the source: position of the name, of its '(' and of the matching ')'.
struct Call
{
  size_t start;
  size_t open;
  size_t close;
};

/**
 * @brief Next call to `marker` (a function name followed by '(') at or after `pos` that is not part of a longer
 *        identifier and whose parentheses are balanced.
 */
[[nodiscard]] inline auto
nextCall(const std::string & code, const std::string & marker, size_t pos) -> std::optional<Call>
{
  for (; (pos = code.find(marker, pos)) != std::string::npos; pos += marker.size())
  {
    if (pos > 0 && isWordChar(code[pos - 1]))
    {
      continue;
    }
    const size_t open = pos + marker.size() - 1;
    const size_t close = findClosing(code, open);
    if (close != std::string::npos)
    {
      return Call{ pos, open, close };
    }
  }
  return std::nullopt;
}

/**
 * @brief Call `edit(start, open, close)` for every kernel signature `<marker> name(...)` whose parameter list is
 *        balanced (`start`: position of the marker, `open` / `close`: its parentheses). `edit` may modify the code
 *        and returns the position to resume the search from.
 */
template <typename Edit>
auto
forEachKernelSignature(std::string & code, const std::string & marker, Edit && edit) -> void
{
  size_t pos = 0;
  while ((pos = code.find(marker, pos)) != std::string::npos)
  {
    const size_t open = code.find('(', pos + marker.size());
    if (open == std::string::npos)
    {
      return;
    }
    const size_t close = findClosing(code, open);
    pos = (close == std::string::npos) ? pos + marker.size() : edit(pos, open, close);
  }
}

} // namespace cle::detail

#endif // __SRC_TRANSLATOR_COMMON_HPP
