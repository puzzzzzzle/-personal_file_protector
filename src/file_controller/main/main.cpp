//
// Created by tao on 19-1-17.
//
#include "common_includes.h"
#include "get_usr_passwd.h"
#include "file_controller.h"
#include "file_controller_adaptor.h"
#include <boost/filesystem.hpp>


TEST(md5_string, hello) {
    EXPECT_STREQ("1", "1");
}

TEST(file_controller, all) {


}

int main(int argc, char **argv) {

    MainFileController *controller =MainFileController::Instance;
    EncINode node;
    node.encName="/";
    node.encFileType=FILE_TYPE_DIR;

    EncINode node1;
    node1.encName="/node1";
    node1.encFileType=FILE_TYPE_FILE;

    controller->table.insert(node);
    controller->table.insert(node1);
    controller->saveTable();
    controller->loadTable();
    std::string fileName = controller->getRandFileName();

    namespace bf=boost::filesystem;
    bf::ofstream file(fileName,std::ios::out|std::ios::trunc|std::ios::binary);
    file<<fileName<<std::endl;


    DEBUG("finish")
}