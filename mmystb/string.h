#ifndef STRING_H
#define STRING_H
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
	char *data;
	size_t len;
	size_t capacity;
} string;

typedef string strSlice;

typedef struct {
	FILE *fp;
	string line;
} Scanner;

const size_t MIN_STR_SIZE = 16;
const strSlice EMPTY_STR = {0};

string newStr(const char *fmt, ...);
string newStrWithCapacity(const size_t initialCapacity, const char *fmt, ...);
string newStrVa(const size_t minSize, const char *fmt, va_list ap);
string newStrFromExisting(const strSlice s);
string newStrFromArray(char *s[], const size_t len, const char *delim);
string newStrSlice(const strSlice s, ssize_t start, ssize_t end, const ssize_t step);

int getFmtSize(const char *fmt, ...);
int getFmtSizeVa(const char *fmt, va_list ap);

void shrinkToFit(string *s);
strSlice sliceStr(const char *s, const size_t len);
strSlice sliceStrRange(const strSlice s, const size_t start, const size_t end);
strSlice sliceStrC(const char *s);
void strClear(string *s);

void strPushc(string *s, const char c);
char strTopc(const strSlice s);
char strPopc(string *s);
void strPushs(string *s, const char *fmt, ...);
void strPush(string *s, const strSlice src);
void strPushcAt(string *s, const size_t n, const char c);
void strPushsAt(string *s, const size_t n, const char *fmt, ...);
bool strIsEmpty(const strSlice s);
int strCmp(const strSlice s1, const strSlice s2);
int strnCmp(const strSlice s1, const strSlice s2, size_t n);
int strnCmpC(const strSlice s1, const char *s2, const size_t n);
int strCmpC(const strSlice s1, const char *s2);
bool strIsEqual(const strSlice s1, const strSlice s2);
bool strIsEqualC(const strSlice s1, const char *s2);
bool strnIsEqual(const strSlice s1, const strSlice s2, const size_t n);
bool strnIsEqualC(const strSlice s1, const char *s2, const size_t n);

strSlice strTokStart(const strSlice s, const char *delim);
strSlice strTok(const strSlice s, const strSlice prevSlice, const char *delim);
void strForEachTok(const strSlice s, const char *delim, void (*action)(const strSlice));

size_t strCountc(const strSlice haystack, const char needle);
size_t strCounts(const strSlice haystack, const strSlice needle);
size_t strCountsC(const strSlice haystack, const char *needle);

ssize_t strChr(const strSlice haystack, const char needle, const size_t startOffset, size_t occurance);
ssize_t strrChr(const strSlice haystack, const char needle, const size_t startOffset, size_t occurance);
ssize_t strStr(const strSlice haystack, const strSlice needle, const size_t startOffset);
ssize_t strStrC(const strSlice haystack, const char *needle, const size_t startOffset);

strSlice strExtractBetween(const strSlice s, const strSlice left, const strSlice right);
strSlice strExtractBetweenC(const strSlice s, const char *left, const char *right);

void strReplace(string *s, const char *sub, const char *by, size_t maxOccurrences);
void strReplaceAll(string *s, const char *sub, const char *by);

void strReverse(string *s);
void strToLower(string *s);
void strToUpper(string *s);
void strTrimCset(string *s, const char *cset);
void strTrim(string *s);

int charToInt(const char c);
long long strToNumeric(const strSlice s);
bool strIsNumeric(const strSlice s);
int strScanf(const strSlice s, const char *fmt, ...);

void strFree(string *s);
void strPrint(const strSlice s);
void strPrintln(const strSlice s);
void strDebugPrint(const strSlice s);

string input(const char *prompt);
string readWholeFile(const char *fileName);

Scanner newScanner(FILE *fp);
const string *scannerNextLine(Scanner *scanner);
void scannerFree(Scanner *scanner);

size_t strDisplayedLength(const strSlice s);

unsigned int getEditDistance(const char *s1, const char *s2);
unsigned int getEditDistanceSlice(const strSlice s1, const strSlice s2);

void swap(void *a, void *b, const size_t size);
size_t getFileSize(FILE *fp);

#ifdef STRING_IMPL
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

int strMax(int a, int b)
{
	return a > b ? a : b;
}

int strMin(int a, int b)
{
	return a > b ? b : a;
}

int strMin3(int a, int b, int c)
{
	return strMin(a, strMin(b, c));
}

string newStr(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	string s = newStrVa(MIN_STR_SIZE, fmt, ap);
	va_end(ap);
	return s;
}

string newStrWithCapacity(const size_t initialCapacity, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	string s = newStrVa(initialCapacity, fmt, ap);
	va_end(ap);
	return s;
}

string newStrVa(const size_t minSize, const char *fmt, va_list ap)
{
	string s;
	va_list ap1;
	va_copy(ap1, ap);

	s.len = getFmtSizeVa(fmt, ap1);
	s.capacity = strMax(s.len * 2, minSize);
	va_end(ap1);

	s.data = malloc(sizeof(char) * s.capacity);
	va_copy(ap1, ap);
	vsnprintf(s.data, s.capacity, fmt, ap1);
	va_end(ap1);
	return s;
}

string newStrFromExisting(const strSlice s)
{
	return newStr("%.*s", s.len, s.data);
}

string newStrFromArray(char *s[], const size_t len, const char *delim)
{
	string str = newStr("%s", *s);
	for (size_t i = 1; i < len; i++)
		strPushs(&str, "%s%s", delim, s[i]);
	return str;
}

void shrinkToFit(string *s)
{
	s->capacity = s->len + 1;
	s->data = realloc(s->data, sizeof(char) * s->capacity);
}

string newStrSlice(const strSlice s, ssize_t start, ssize_t end, const ssize_t step)
{
	if (start < 0)
		start += s.len;
	if (end < 0)
		end += s.len;
	assert(step != 0);

	string slice = newStr("");
	ssize_t i = start;
	while (i != (ssize_t)end && i >= 0 && (size_t)i < s.len) {
		strPushc(&slice, s.data[i]);
		i += step;
	}
	strPushc(&slice, s.data[i]);
	strPushc(&slice, '\0');
	return slice;
}

strSlice sliceStr(const char *s, const size_t len)
{
	strSlice slice;
	slice.data = (char*)s;
	slice.len = len;
	return slice;
}

strSlice sliceStrRange(const strSlice s, const size_t start, const size_t end)
{
	strSlice slice;
	slice.data = s.data + start;
	slice.len = end - start + 1;
	return slice;
}

strSlice sliceStrC(const char *s)
{
	return sliceStr(s, strlen(s));
}

void strClear(string *s)
{
	s->capacity = MIN_STR_SIZE;
	s->data = realloc(s->data, sizeof(char) * s->capacity);
	s->len = 0;
}

void strPushc(string *s, const char c)
{
	s->len++;
	if (s->len + 1 >= s->capacity) {
		s->capacity *= 2;
		s->data = realloc(s->data, sizeof(char) * s->capacity);
	}
	s->data[s->len - 1] = c;
	s->data[s->len] = '\0';
}

char strTopc(const strSlice s)
{
	return s.data[s.len - 1];
}

char strPopc(string *s)
{
	char c = strTopc(*s);
	s->data[--s->len - 1] = '\0';
	return c;
}

void strPushs(string *s, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	const int len = getFmtSizeVa(fmt, ap) + 1;
	char buf[len];
	vsnprintf(buf, len, fmt, ap);
	va_end(ap);

	if (s->len + len + 1 >= s->capacity) {
		s->capacity = ((s->len + len) * 2);
		s->data = realloc(s->data, sizeof(char) * s->capacity);
	}

	strcpy(&s->data[s->len], buf);
	s->len += len - 1;
}

void strPush(string *s, const strSlice src)
{
	strPushs(s, "%.*s", src.len, src.data);
}

void strPushcAt(string *s, const size_t n, const char c)
{
	s->len++;
	if (s->len + 1 >= s->capacity) {
		s->capacity *= 2;
		s->data = realloc(s->data, sizeof(char) * s->capacity);
	}
	memmove(&s->data[n+1], &s->data[n], s->len - n - 1);
	s->data[n] = c;
}

void strPushsAt(string *s, const size_t n, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	const int len = getFmtSizeVa(fmt, ap) + 1;
	char buf[len];
	vsnprintf(buf, len, fmt, ap);
	va_end(ap);

	if (s->len + len + 1 >= s->capacity) {
		s->capacity = ((s->len + len) * 2);
		s->data = realloc(s->data, sizeof(char) * s->capacity);
	}

	memmove(&s->data[n+len-1], &s->data[n], s->len + len - n);
	memcpy(&s->data[n], buf, len - 1);
	s->len += len - 1;
}

bool strIsEmpty(const strSlice s)
{
	return s.len == 0;
}

int strCmp(const strSlice s1, const strSlice s2)
{
	if (s1.len > s2.len)
		return 1;
	if (s1.len < s2.len)
		return -1;
	return strncmp(s1.data, s2.data, s1.len);
}

int strnCmp(const strSlice s1, const strSlice s2, size_t n)
{
	return strncmp(s1.data, s2.data, n);
}

int strnCmpC(const strSlice s1, const char *s2, const size_t n)
{
	return strncmp(s1.data, s2, n);
}

int strCmpC(const strSlice s1, const char *s2)
{
	return strnCmpC(s1, s2, s1.len);
}

bool strIsEqual(const strSlice s1, const strSlice s2)
{
	return strCmp(s1, s2) == 0;
}

bool strIsEqualC(const strSlice s1, const char *s2)
{
	return strCmpC(s1, s2) == 0;
}

bool strnIsEqual(const strSlice s1, const strSlice s2, const size_t n)
{
	return strnCmp(s1, s2, n) == 0;
}

bool strnIsEqualC(const strSlice s1, const char *s2, const size_t n)
{
	return strnCmpC(s1, s2, n) == 0;
}

strSlice strTokStart(const strSlice s, const char *delim)
{
	strSlice slice = sliceStr(s.data, 0);
	return strTok(s, slice, delim);
}

strSlice strTok(const strSlice s, const strSlice prevSlice, const char *delim)
{
	size_t start = prevSlice.data + prevSlice.len - s.data;

	while (start < s.len && strchr(delim, s.data[start]) != NULL)
		start++;

	size_t end = start;
	while (end < s.len && strchr(delim, s.data[end]) == NULL)
		end++;

	return sliceStrRange(s, start, end - 1);
}

void strForEachTok(const strSlice s, const char *delim, void (*action)(const strSlice))
{
	strSlice slice = strTokStart(s, delim);
	while (!strIsEmpty(slice)) {
		action(slice);
		slice = strTok(s, slice, delim);
	}
}

size_t strCountc(const strSlice haystack, const char needle)
{
	size_t count = 0;
	for (size_t i = 0; i < haystack.len; i++)
		if (haystack.data[i] == needle)
			count++;
	return count;
}

size_t strCounts(const strSlice haystack, const strSlice needle)
{
	ssize_t count = 0;
	ssize_t i = -1;
	while ((i = strStr(haystack, needle, i + 1)) != -1)
		count++;
	return count;
}

size_t strCountsC(const strSlice haystack, const char *needle)
{
	ssize_t count = 0;
	ssize_t i = -1;
	while ((i = strStrC(haystack, needle, i + 1)) != -1)
		count++;
	return count;
}

ssize_t strChr(const strSlice haystack, const char needle, const size_t startOffset, size_t occurance)
{
	size_t i = startOffset;
	while (i < haystack.len && occurance > 0) {
		if (haystack.data[i] == needle)
			occurance--;
		i++;
	}
	if (occurance == 0)
		return i;
	return -1;
}

ssize_t strrChr(const strSlice haystack, const char needle, const size_t startOffset, size_t occurance)
{
	ssize_t i = startOffset;
	while (i >= 0 && occurance > 0) {
		if (haystack.data[i] == needle)
			occurance--;
		i--;
	}
	if (occurance == 0)
		return i;
	return -1;
}

ssize_t strStr(const strSlice haystack, const strSlice needle, const size_t startOffset)
{
	for (size_t i = startOffset; i < haystack.len - needle.len + 1; i++)
		if (strIsEqual(sliceStrRange(haystack, i, i + needle.len - 1), needle))
			return i;
	return -1;
}

ssize_t strStrC(const strSlice haystack, const char *needle, const size_t startOffset)
{
	return strStr(haystack, sliceStrC(needle), startOffset);
}

strSlice strExtractBetween(const strSlice s, const strSlice left, const strSlice right)
{
	ssize_t start = strStr(s, left, 0);
	if (start == -1)
		return EMPTY_STR;
	start += left.len;
	ssize_t end = strStr(s, right, start);
	if (end == -1)
		return EMPTY_STR;
	return sliceStrRange(s, start, end - 1);
}

strSlice strExtractBetweenC(const strSlice s, const char *left, const char *right)
{
	return strExtractBetween(s, sliceStrC(left), sliceStrC(right));
}

void strReplaceAll(string *s, const char *sub, const char *by)
{
	strReplace(s, sub, by, s->len);
}

void strReplace(string *s, const char *sub, const char *by, size_t maxOccurrences)
{
	string result = newStr("");
	size_t i = 0;
	const int sublen = strlen(sub);

	while (i < s->len - sublen + 1 && maxOccurrences > 0) {
		strSlice tmp = sliceStrRange(*s, i, i + sublen - 1);
		if (strIsEqualC(tmp, sub)) {
			strPushs(&result, "%s", by);
			i += sublen;
			maxOccurrences--;
		} else {
			strPushc(&result, s->data[i++]);
		}
	}

	// push the rest of the string
	strPushs(&result, "%.*s", s->len - i, s->data + i);

	strFree(s);
	*s = result;
}

void strCut(string *s, const size_t start, const size_t end)
{
	strSlice left = sliceStrRange(*s, 0, start - 1);
	strSlice right = sliceStrRange(*s, end + 1, s->len - 1);
	string tmp = newStr("");
	strPush(&tmp, left);
	strPush(&tmp, right);
	strFree(s);
	*s = tmp;
}

void strReverse(string *s)
{
	int left = 0;
	int right = s->len - 1;
	while (left < right)
		swap(&s->data[left++], &s->data[right--], sizeof(char));
}

void strToLower(string *s)
{
	for (size_t i = 0; i < s->len; ++i)
		s->data[i] = tolower(s->data[i]);
}

void strToUpper(string *s)
{
	for (size_t i = 0; i < s->len; ++i)
		s->data[i] = toupper(s->data[i]);
}

int charToInt(const char c)
{
	return c - '0';
}

long long strToNumeric(const strSlice s)
{
	long long num = 0;
	for (size_t i = 0; i < s.len; i++) {
		num *= 10;
		num += charToInt(s.data[i]);
	}
	return num;
}

bool strIsNumeric(const strSlice s)
{
	for (size_t i = 0; i < s.len; ++i)
		if (!isdigit(s.data[i]))
			return false;
	return true;
}

void strTrimCset(string *s, const char *cset)
{
	size_t start = 0;
	size_t end = s->len - 1;

	while (start < s->len && strchr(cset, s->data[start]))
		start++;

	while (start < s->len && strchr(cset, s->data[end]))
		end--;

	const int newLen = end - start + 1;
	memmove(s->data, s->data + start, newLen);
	s->len = newLen;
	s->data[newLen] = '\0';
}

void strTrim(string *s)
{
	strTrimCset(s, "\f\n\r\t\v");
}

int strScanf(const strSlice s, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	int ret = vsscanf(s.data, fmt, ap);
	va_end(ap);
	return ret;
}

void strFree(string *s)
{
	assert(s->data != NULL);
	free(s->data);
}

void strPrint(const strSlice s)
{
	printf("%.*s", (int)s.len, s.data);
}

void strPrintln(const strSlice s)
{
	printf("%.*s\n", (int)s.len, s.data);
}

void strDebugPrint(const strSlice s)
{
	printf("len: %zu\ncapacity: %zu\ndata: %.*s\n", s.len, s.capacity, (int)s.len, s.data);
}

string input(const char *prompt)
{
	string s = newStr("");
	printf("%s", prompt);
	char c;
	while ((c = fgetc(stdin)) != '\n')
		strPushc(&s, c);
	return s;
}

string readWholeFile(const char *fileName)
{
	FILE *fp = fopen(fileName, "r");
	if (fp == NULL)
		return EMPTY_STR;
	const size_t fileSize = getFileSize(fp);
	string s = newStrWithCapacity(fileSize + 2, "");
	s.len = fread(s.data, sizeof(char), fileSize, fp);
	s.data[s.len] = '\0';
	fclose(fp);
	return s;
}

Scanner newScanner(FILE *fp)
{
	Scanner scanner;
	scanner.fp = fp;
	scanner.line.data = NULL;
	scanner.line.len = 0;
	scanner.line.capacity = 0;
	return scanner;
}

const string *scannerNextLine(Scanner *scanner)
{
	ssize_t len = getline(&scanner->line.data, &scanner->line.capacity, scanner->fp);
	if (len == -1)
		return NULL;
	scanner->line.len = len;
	if (scanner->line.len > 0 && scanner->line.data[scanner->line.len - 1] == '\n')
		scanner->line.data[scanner->line.len - 1] = '\0';
	return &scanner->line;
}

void scannerFree(Scanner *scanner)
{
	free(scanner->line.data);
}

size_t strDisplayedLength(const strSlice s)
{
	bool escapeSeq = false;
	size_t len = 0;

	for (size_t i = 0; i < s.len; i++) {
		const char c = s.data[i];
		if (c == '\033')
			escapeSeq = true;
		else if (escapeSeq == true && c == 'm')
			escapeSeq = false;
		else if (escapeSeq)
			continue;
		else if (c == '\t')
			len += 8;
		else
			len++;
	}

	return len;
}

unsigned int getEditDistance(const char *s1, const char *s2)
{
	return getEditDistanceSlice(sliceStrC(s1), sliceStrC(s2));
}

unsigned int getEditDistanceSlice(const strSlice s1, const strSlice s2)
{
	unsigned int row[s1.len + 1];

	for (unsigned int j = 0; j <= s1.len; j++)
		row[j] = j;

	for (unsigned int i = 1; i <= s2.len; i++) {
		row[0] = i;
		unsigned int topleft = i - 1;
		for (unsigned int j = 1; j <= s1.len; j++) {
			unsigned int curr;
			if (tolower(s1.data[j-1]) == tolower(s2.data[i-1]))
				curr = topleft;
			else
				curr = 1 + strMin3(topleft, row[j], row[j-1]);
			topleft = row[j];
			row[j] = curr;
		}
	}
	return row[s1.len];
}

#ifndef SWAP
#define SWAP
void swap(void *a, void *b, const size_t size)
{
	char tmp[size];
	memcpy(tmp, a, size);
	memcpy(a, b, size);
	memcpy(b, tmp, size);
}
#endif
#ifndef GET_FILE_SIZE
#define GET_FILE_SIZE
size_t getFileSize(FILE *fp)
{
	const size_t curr = ftell(fp);
	fseek(fp, 0, SEEK_END);
	const size_t size = ftell(fp);
	fseek(fp, curr, SEEK_SET);
	return size;
}
#endif
#ifndef GET_FMT_SIZE
#define GET_FMT_SIZE
int getFmtSize(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	const int size = getFmtSizeVa(fmt, ap);
	va_end(ap);
	return size;
}
#endif
#ifndef GET_FMT_SIZE_VA
#define GET_FMT_SIZE_VA
int getFmtSizeVa(const char *fmt, va_list ap)
{
	va_list ap1;
	va_copy(ap1, ap);
	const int size = vsnprintf(NULL, 0, fmt, ap1);
	va_end(ap1);
	return size;
}
#endif
#endif
#endif
