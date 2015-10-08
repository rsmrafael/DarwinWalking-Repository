/*
 * DebugDrawings.cpp
 *
 *  Created on: 15.01.2013
 *      Author: Stefan Krupop
 */

#include "DebugDrawings.h"
#include "../Messages/DebugDrawings.pb.h"
#include "../Messages/OstreamOutputStream.h"
#include "../Messages/IstreamInputStream.h"

using namespace std;

DebugDrawings::DebugDrawings() {
	mData = new vector<DebugDrawing>();
	mTextData = new vector<DebugText>();
}

DebugDrawings::~DebugDrawings() {
	delete mData;
	delete mTextData;
}

void DebugDrawings::clear(){
	mData->clear();
	mTextData->clear();
}

void DebugDrawings::add(DebugDrawing drawing) {
	mData->push_back(drawing);
}

void DebugDrawings::add(DebugText text) {
	mTextData->push_back(text);
}

const vector<DebugDrawings::DebugDrawing>* DebugDrawings::getData() const {
	return mData;
}

const vector<DebugDrawings::DebugText>* DebugDrawings::getTextData() const {
	return mTextData;
}

void Serializer<DebugDrawings>::serialize(const DebugDrawings& representation, ostream& stream) {
	protobuf::DebugDrawings DebugDrawingsMsg;

	const vector<DebugDrawings::DebugDrawing>* lst = representation.getData();
	for(vector<DebugDrawings::DebugDrawing>::const_iterator it = lst->begin(); it != lst->end(); ++it){
		protobuf::DebugDrawings_DebugDrawing* ob = DebugDrawingsMsg.add_drawing();
		ob->set_module((*it).module);
		ob->set_type((protobuf::DebugDrawings_DebugDrawing_DrawShape)(*it).type);
		ob->set_x1((*it).x1);
		ob->set_y1((*it).y1);
		ob->set_p1((*it).p1);
		ob->set_p2((*it).p2);
		ob->set_pencolor((protobuf::DebugDrawings_DrawColor)(*it).penColor);
	}

	const vector<DebugDrawings::DebugText>* textData = representation.getTextData();
	for(vector<DebugDrawings::DebugText>::const_iterator it = textData->begin(); it != textData->end(); ++it){
		protobuf::DebugDrawings_DebugText* ob = DebugDrawingsMsg.add_text();
		ob->set_module((*it).module);
		ob->set_text((*it).text);
		ob->set_x((*it).x);
		ob->set_y((*it).y);
		ob->set_pencolor((protobuf::DebugDrawings_DrawColor)(*it).penColor);
	}

	OstreamOutputStream buf(&stream);
	DebugDrawingsMsg.SerializeToZeroCopyStream(&buf);
}

void Serializer<DebugDrawings>::deserialize(istream& stream, DebugDrawings& representation) {
	protobuf::DebugDrawings DebugDrawingsMsg;

	IstreamInputStream buf(&stream);
	DebugDrawingsMsg.ParseFromZeroCopyStream(&buf);

	representation.clear();
}
