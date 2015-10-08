/*
 * PBImage.cpp
 *
 *  Created on: 16.02.2012
 *      Author: Stefan
 */

#include "PBImage.h"
#include "../../Debugging/Debugger.h"
#include "../../Messages/ProtoBufFrame.pb.h"
#include "../../Representations/YUVImage.h"
#include <iostream>
#include <fstream>

using namespace std;

PBImage::PBImage(string fileName) {
	//Debugger::INFO("PBImage", "PBF: %s", fileName.c_str());
	mLoadedFrame = new protobuf::ProtoBufFrame();
	fstream input(fileName.c_str(), ios::in | ios::binary);

	// get length of file:
	input.seekg((streamoff) 0, ios::end);
	streamsize datasize = (streamsize) input.tellg();
	input.seekg((streamoff) 0, ios::beg);

	char* data = (char*) malloc((size_t) datasize);
	input.read(data, datasize);
	if (!mLoadedFrame->ParseFromArray(data, (int) datasize)) {
		Debugger::ERR("PBImage", "Could not load PBF '%s'", fileName.c_str());
		delete mLoadedFrame;
		mLoadedFrame = NULL;
	}

	free(data);
	input.close();
}

PBImage::~PBImage() {
	delete mLoadedFrame;
}

YUVImage* PBImage::getImage(int id) const {
	protobuf::CameraLocation location = protobuf::HEAD_LEFT;
	if (id == 0) {
		location = protobuf::HEAD_LEFT;
	} else if (id == 1) {
		location = protobuf::HEAD_RIGHT;
	}

	if (mLoadedFrame != NULL) {
		for (int i = 0; i < mLoadedFrame->camera_size(); i++) {
			const protobuf::ProtoBufFrame_Camera& cam = mLoadedFrame->camera(i);

			if (cam.type().location() == location) {

				const protobuf::ImageData& img = cam.image_data();

				if (img.format() == protobuf::YUV422_IMAGE) {
					int width = img.width();
					int height = img.height();
					const char* data = img.data().data();
					YUVImage* yuvImg = new YUVImage(data, width, height);
					return yuvImg;
				}
			}
		}
	}

	return NULL;
}

ObjectList PBImage::getAnnotations(int id) const {

	ObjectList annotations = ObjectList();
	protobuf::CameraLocation location = protobuf::HEAD_LEFT;
	if (id == 0) {
		location = protobuf::HEAD_LEFT;
	} else if (id == 1) {
		location = protobuf::HEAD_RIGHT;
	}

	if (mLoadedFrame != NULL) {
		for (int i = 0; i < mLoadedFrame->camera_size(); i++) {
			const protobuf::ProtoBufFrame_Camera& cam = mLoadedFrame->camera(i);

			if (cam.type().location() == location) {

				for (int o = 0; o < cam.objects_size(); ++o) {
					const protobuf::ProtoBufFrame_Object obj = cam.objects(o);
					Object::ObjectType type;

					switch (obj.type()) {
					case protobuf::BALL:
						type = Object::BALL;
						break;
					case protobuf::GOAL_POLE_YELLOW:
						type = Object::GOAL_POLE_YELLOW;
						break;
					case protobuf::GOAL_YELLOW_CROSSBAR:
						type = Object::GOAL_YELLOW_CROSSBAR;
						break;
					case protobuf::ROBOT:
						type = Object::ROBOT;
						break;
					case protobuf::ROBOT_CYAN:
						type = Object::ROBOT_CYAN;
						break;
					case protobuf::ROBOT_MAGENTA:
						type = Object::ROBOT_MAGENTA;
						break;
					case protobuf::LINE:
						type = Object::LINE;
						break;
					case protobuf::LINE_POINT:
						type = Object::LINE_POINT;
						break;
					case protobuf::OBSTACLE:
						type = Object::OBSTACLE;
						break;
					case protobuf::FIELD_LINE:
						type = Object::FIELD_LINE;
						break;
					case protobuf::UNKNOWN:
					default:
						Debugger::DEBUG("PBImage","Unsupported ObjectType!");
						type = Object::UNKNOWN;
						break;
					}

					Object ob = Object(obj.box().x1(), obj.box().y1(),
							obj.box().x2(), obj.box().y2(), type);
					annotations.addObject(ob);
				}
			}
		}
	}

	return annotations;
}

bool PBImage::getPanTilt(int id, int* pan, int* tilt) const {
	protobuf::CameraLocation location = protobuf::HEAD_LEFT;
	if (id == 0) {
		location = protobuf::HEAD_LEFT;
	} else if (id == 1) {
		location = protobuf::HEAD_RIGHT;
	}

	if (mLoadedFrame != NULL) {
		for (int i = 0; i < mLoadedFrame->camera_size(); i++) {
			const protobuf::ProtoBufFrame_Camera& cam = mLoadedFrame->camera(i);

			if (cam.type().location() == location) {
				const protobuf::Vector& Pos = cam.camera_position();
				if (Pos.IsInitialized()) {
					*pan = Pos.x();
					*tilt = Pos.y();
					return true;
				} else {
					//Debugger::WARN("PBImage", "no Pan/Tilt info!");
				}
			}
		}
	}
	*pan = 0;
	*tilt = 0;
	return false;
}

bool PBImage::writePBI(string fileName, const YUVImage& image, int pan, int tilt) {
	protobuf::ProtoBufFrame frame;

	protobuf::ProtoBufFrame_Camera* camera = frame.add_camera();
	protobuf::ProtoBufFrame_Camera_CameraType* camera_type =
			camera->mutable_type();
	camera_type->set_location(protobuf::HEAD_LEFT);
	camera_type->set_name("unknown");
	camera_type->set_sensor_size_width(1600.0 * 0.0022);
	camera_type->set_sensor_size_height(1200.0 * 0.0022);

	protobuf::Vector* camera_position = camera->mutable_camera_position();
	camera_position->set_x(pan);
	camera_position->set_y(tilt);
	camera_position->set_z(0);

	protobuf::ImageData* imageData = camera->mutable_image_data();
	imageData->set_format(protobuf::YUV422_IMAGE);

	imageData->set_data(image.getImage(),
			(size_t) (uint32_t) (image.getHeight() * image.getWidth() * 2));

	imageData->set_compressed(false);
	imageData->set_width(image.getWidth());
	imageData->set_height(image.getHeight());

	fstream output(fileName.c_str(), ios::out | ios::trunc | ios::binary);
	if (!output.good()) {
		Debugger::ERR("PBImage", "Could not open output stream");
	}
	if (!frame.IsInitialized()) {
		Debugger::ERR("PBImage", "ProtoBuf is not initialized!");
		string missing = frame.InitializationErrorString();
		if (missing != "") {
			Debugger::ERR("PBImage", "Missing field in annotation: %s",
					missing.c_str());
		}
	}
	size_t datasize = (size_t) (uint32_t) (frame.ByteSize());
	char* data = (char*) malloc(datasize);
	if (!frame.SerializeToArray(data, (int) datasize)) {
		Debugger::ERR("PBImage", "Failed to write PBI!");
		free(data);
		output.flush();
		output.close();
		return false;
	}
	output.write(data, (streamsize) datasize);
	free(data);
	output.flush();
	output.close();
	Debugger::INFO("PBImage", "Successfully wrote PBI %s!", fileName.c_str());
	return true;
}

void PBImage::shutdown(void) {
	google::protobuf::ShutdownProtobufLibrary();
}
