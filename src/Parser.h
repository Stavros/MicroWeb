//
// Copyright (C) 2021 James Howard
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//

#ifndef _PARSER_H_
#define _PARSER_H_

#include <stdint.h>
#include "Nodes/Section.h"
#include "Stack.h"

class Page;
class Node;
struct Image;
class HTMLTagHandler;

struct TextEncoding
{
	enum Type
	{
		UTF8,
		ISO_8859_1,
		ISO_8859_2
	};
};

struct HTMLParseContext
{
	Node* node;
	const HTMLTagHandler* tag;
	SectionElement::Type parseSection;
};

class AttributeParser
{
public:
	AttributeParser(char* inAttributeString) : attributeString(inAttributeString), key(NULL), value(NULL) {}
	bool Parse();
	
	const char* Key() { return key; }
	const char* Value() { return value; }
	
private:
	bool IsWhiteSpace(char c);
	
	char* attributeString;
	char* key;
	char* value;
};

class HTMLParser
{
public:
	HTMLParser(Page& page);

	void Reset();
	void Parse(char* buffer, size_t count);

	Page& page;


	void PushContext(Node* node, const HTMLTagHandler* tag);
	void PopContext(const HTMLTagHandler* tag);
	HTMLParseContext& CurrentContext() { return contextStack.Top(); }
	SectionElement::Type CurrentSection() { return CurrentContext().parseSection; }

	void EmitNode(Node* node);
	void EmitText(const char* text);
	void EmitImage(Image* image, int imageWidth, int imageHeight);

	void SetTextEncoding(TextEncoding::Type newType);

	void PushPreFormatted();
	void PopPreFormatted();

	static uint8_t ParseColourCode(const char* colourCode);

	static void ReplaceAmpersandEscapeSequences(char* buffer, bool replaceNonBreakingSpace = true);

private:
	void ParseChar(char c);

	//HTMLNode* CreateNode(HTMLNode::NodeType nodeType, HTMLNode* parentNode);
	void AppendTextBuffer(char c);
	void FlushTextBuffer();
	static bool IsWhiteSpace(char c);

	void DebugDumpNodeGraph(Node* node, int depth = 0);

	enum ParseState
	{
		ParseText,
		ParsePossibleTag,
		ParseTag,
		ParseAmpersandEscape,
		ParseComment
	};
	
	ParseState parseState;
	char textBuffer[2560];
	size_t textBufferSize;
	int escapeSequenceStartIndex;
	
	Stack<HTMLParseContext> contextStack;
	int contextStackSize;

	bool parsingUnicode;
	int unicodeByteCount;
	uint32_t unicodePoint;

	TextEncoding::Type textEncoding;

	unsigned int preformatted;
};

#endif
