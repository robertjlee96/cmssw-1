// -*- C++ -*-
//
// Package:     ParameterSet
// Class  :     ParameterDescriptionBase
//
// Implementation:
//     <Notes on implementation>
//
// Original Author:  Chris Jones
//         Created:  Thu Aug  2 15:35:43 EDT 2007
//

#include "FWCore/ParameterSet/interface/ParameterDescriptionBase.h"
#include "FWCore/ParameterSet/interface/DocFormatHelper.h"
#include "FWCore/Utilities/interface/EDMException.h"

#include <iostream>
#include <iomanip>

namespace edm {

  ParameterDescriptionBase::ParameterDescriptionBase(std::string const& iLabel,
                                                     ParameterTypes iType,
                                                     bool isTracked)
    :label_(iLabel),
     type_(iType),
     isTracked_(isTracked)
  {
    if(label_.empty()) {
      throw edm::Exception(errors::Configuration)
        << "Empty string used as a label for a parameter.  This is\n"
           "not allowed.\n";
    }
  }

  ParameterDescriptionBase::ParameterDescriptionBase(char const* iLabel,
                                                     ParameterTypes iType,
                                                     bool isTracked)
    :label_(iLabel),
     type_(iType),
     isTracked_(isTracked)
  {
    if (label_.empty()) {
      throw edm::Exception(errors::Configuration)
        << "Empty string used as a label for a parameter.  This is\n"
           "not allowed.\n";
    }
  }

  ParameterDescriptionBase::~ParameterDescriptionBase() { }

  void
  ParameterDescriptionBase::throwParameterWrongTrackiness() const {
    std::string tr("a tracked");
    std::string shouldBe("untracked");
    if (isTracked()) {
      tr = "an untracked";
      shouldBe = "tracked";
    }

    throw edm::Exception(errors::Configuration)
      << "In the configuration, parameter \"" << label() << "\" is defined "
      "as " << tr << " " << parameterTypeEnumToString(type()) << ".\n"
      << "It should be " << shouldBe << ".\n";
  }

  void
  ParameterDescriptionBase::throwParameterWrongType() const {
    std::string tr("an untracked");
    if (isTracked()) tr = "a tracked";

    throw edm::Exception(errors::Configuration)
      << "Parameter \"" << label() << "\" should be defined "
      "as " << tr << " " << parameterTypeEnumToString(type()) << ".\n"
      << "The type in the configuration is incorrect.\n";
  }

  void
  ParameterDescriptionBase::
  checkAndGetLabelsAndTypes_(std::set<std::string> & usedLabels,
                             std::set<ParameterTypes> & parameterTypes,
                             std::set<ParameterTypes> & wildcardTypes) const {
    usedLabels.insert(label());
    parameterTypes.insert(type());
  }

  void
  ParameterDescriptionBase::
  writeCfi_(std::ostream & os,
           bool & startWithComma,
           int indentation,
           bool & wroteSomething) const {

    wroteSomething = true;
    if (startWithComma) os << ",";
    startWithComma = true;

    os << "\n" << std::setfill(' ') << std::setw(indentation) << "";

    os << label()
       << " = cms.";
    if (!isTracked()) os << "untracked.";
    os << parameterTypeEnumToString(type())
       << "(";
    writeCfi_(os, indentation);
    os << ")";
  }

  void
  ParameterDescriptionBase::
  print_(std::ostream & os,
         bool optional,
	 bool writeToCfi,
         DocFormatHelper & dfh)
  {
    if (dfh.pass() == 0) {
      dfh.setAtLeast1(label().size());
      dfh.setAtLeast2(parameterTypeEnumToString(type()).size());
      dfh.setAtLeast3(9U);
      dfh.setAtLeast4(8U);
    }
    else {

      if (dfh.brief()) {

        dfh.indent(os);
        os << std::left << std::setw(dfh.column1()) << label() << " ";
        os << std::setw(dfh.column2()) << parameterTypeEnumToString(type());
        os << " ";

        os << std::setw(dfh.column3());
        if (isTracked()) os << "tracked";
        else  os << "untracked";
        os << " ";

        os << std::setw(dfh.column4());
        if (optional)  os << "optional ";
        else  os << "required ";

        printThirdLine_(os, writeToCfi, dfh);
      }
      // not brief
      else {

        dfh.indent(os);
        os << label() << "\n";

        dfh.indent2(os);
        os << "type: " << parameterTypeEnumToString(type()) << " ";

        if (isTracked()) os << "tracked ";
        else  os << "untracked ";

        if (optional)  os << "optional\n";
        else  os << "required\n";

        dfh.indent2(os);
        printThirdLine_(os, writeToCfi, dfh);

        if (!comment().empty()) {
          DocFormatHelper::wrapAndPrintText(os,
                                            comment(),
                                            dfh.startColumn2(),
                                            dfh.commentWidth());
        }
        os << "\n";
      }
      os << std::right;
    }
  }

  void
  ParameterDescriptionBase::
  printThirdLine_(std::ostream & os,
                  bool writeToCfi,
                  DocFormatHelper & dfh) {
    if (!dfh.brief()) os << "default: ";
    if (writeToCfi) {
      if (hasNestedContent()) {
        os << "see Section " << dfh.section()
           << "." << dfh.counter();
      }
      else {
        if (dfh.brief()) {
          writeDoc_(os, dfh.indentation());
        }
        else {
          writeDoc_(os, dfh.startColumn2());
        }
      }
    }
    else {
      os << "none (do not write to cfi)";
    }
    os << "\n";
  }

  void
  ParameterDescriptionBase::
  printNestedContent_(std::ostream & os,
                      bool optional,
                      DocFormatHelper & dfh) {
    int indentation = dfh.indentation();
    if (dfh.parent() != DocFormatHelper::TOP) {
      indentation -= DocFormatHelper::offsetSectionContent();
    }

    os << std::setfill(' ') << std::setw(indentation) << "";
    os << "Section " << dfh.section() << "." << dfh.counter()
       << " " << label() << " default contents: ";
    writeDoc_(os, indentation + 2);
    os << "\n";
    if (!dfh.brief()) os << "\n";
  }

  bool
  ParameterDescriptionBase::
  partiallyExists_(ParameterSet const& pset) const {
    return exists(pset);
  }

  int
  ParameterDescriptionBase::
  howManyXORSubNodesExist_(ParameterSet const& pset) const {
    return exists(pset) ? 1 : 0; 
  }
}
