#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/XmlOutputter.h>
#include <cppunit/TextTestRunner.h>

using namespace std;

int main(int argc, char* argv[]) {
	CPPUNIT_NS::TestResult testresult;

	CPPUNIT_NS::TestResultCollector collectedresults;
	testresult.addListener(&collectedresults);

	CPPUNIT_NS::BriefTestProgressListener progress;
	testresult.addListener(&progress);

	CPPUNIT_NS::TestRunner testrunner;
	testrunner.addTest(CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest());
	testrunner.run(testresult);

	cout << endl << flush;

	std::ofstream ofile;
	ofile.open("cppunit_result.xml");
	CPPUNIT_NS::XmlOutputter outputter(&collectedresults, ofile);
	outputter.write();
	ofile.close();

	CPPUNIT_NS::CompilerOutputter compileroutputter(&collectedresults, std::cout);
	compileroutputter.write();

	// return 0 if tests were successful
	return collectedresults.wasSuccessful() ? 0 : 1;
}
