// #################################
// # Author: Timon Heim
// # Email: timon.heim at cern.ch
// # Project: Yarr
// # Description: 1D Histogram
// # Comment: 
// ################################

#include "Histo1d.h"

#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>

#include "storage.hpp"

#include "logging.h"

namespace {
    auto hlog = logging::make_log("Histo1d");
}

Histo1d::Histo1d(std::string arg_name, unsigned arg_bins, double arg_xlow, double arg_xhigh, std::type_index t) : HistogramBase(arg_name, t) {
    bins = arg_bins;
    xlow = arg_xlow;
    xhigh = arg_xhigh;
    binWidth = (xhigh - xlow)/bins;
    data =  std::vector<double>(bins,0);
    min = 0;
    max = 0;

    underflow = 0;
    overflow = 0;
    entries = 0;
    sum = 0;
}

Histo1d::Histo1d(std::string arg_name, unsigned arg_bins, double arg_xlow, double arg_xhigh, std::type_index t, LoopStatus &stat) : HistogramBase(arg_name, t, stat) {
    bins = arg_bins;
    xlow = arg_xlow;
    xhigh = arg_xhigh;
    binWidth = (xhigh - xlow)/bins;
    data = std::vector<double>(bins,0);

    min = 0;
    max = 0;

    underflow = 0;
    overflow = 0;
    entries = 0;
    sum = 0;
}

Histo1d::~Histo1d() {

}

unsigned Histo1d::size() const {
    return bins;
}

unsigned Histo1d::getEntries() const {
    return entries;
}

double Histo1d::getMean() {
    if (sum == 0 || entries == 0)
        return 0;
    double weighted_sum = 0;
    double n = 0;
    for (unsigned i=0; i<bins; i++) {
        weighted_sum += data[i]*(((i)*binWidth)+xlow+(binWidth/2.0));
        n += data[i];
    }
    if (n == 0) {
        return 0;
    }
    return weighted_sum/n;
}

double Histo1d::getStdDev() {
    if (sum == 0 || entries == 0)
        return 0;
    double mean = this->getMean();
    double mu = 0;
    for (unsigned i=0; i<bins; i++)
        mu += data[i] * pow((((i)*binWidth)+xlow+(binWidth/2.0))-mean,2);
    return sqrt(mu/(double)sum);
}

void Histo1d::fill(double x, double v) {
    if (x < xlow) {
        underflow+=v;
    } else if (xhigh <= x) {
        overflow+=v;
    } else {
        unsigned bin = (x-xlow)/binWidth;
        data[bin]+=v;
        if (v > max)
            max = v;
        if (v < min)
            min = v;
        entries++;
        sum+=v;
    }
}

void Histo1d::setBin(unsigned n, double v) {
    if (n < bins) {
        data[n] = v;
        if (v > max)
            max = v;
        if (v < min)
            min = v;
    }
    entries++;
    sum+=v;
}

double Histo1d::getBin(unsigned n) const {
    if (n < bins) {
        return data[n];
    }
    return 0;
}

void Histo1d::scale(const double s) {
    for (unsigned int i=0; i<bins; i++) {
        data[i] = data[i] * s;
    }
    overflow = overflow*s;
    underflow = underflow*s;
    sum = sum*s;
}

void Histo1d::add(const Histo1d &h) {
    if (h.size() != bins) {
        return;
    } else {
        for (unsigned i=0; i<bins; i++) {
            data[i] += h.getBin(i);
            sum += h.getBin(i);
        }
        entries += h.getEntries();
        overflow += h.getOverflow();
        underflow += h.getUnderflow();
    }
}


void Histo1d::toFile(std::string prefix, std::string dir, bool jsonType) {
    std::string filename = dir + prefix + "_" + HistogramBase::name;
    if (jsonType) {
        filename += ".json";
    } else {
        filename += ".dat";
    }
    std::fstream file(filename, std::fstream::out | std::fstream::trunc);
    json j;
    // jsonType
    if (jsonType) {
        j["Type"] = "Histo1d";
        j["Name"] = name;
        
        j["x"]["AxisTitle"] = xAxisTitle;
        j["x"]["Bins"] = bins;
        j["x"]["Low"] = xlow;
        j["x"]["High"] = xhigh;
        
        j["y"]["AxisTitle"] = yAxisTitle;
        
        j["z"]["AxisTitle"] = zAxisTitle;
        
        j["Underflow"] = underflow;
        j["Overflow"] = overflow;

        for (unsigned int i=0; i<bins; i++) 
            j["Data"][i] = data[i];

        file << std::setw(4) << j;
    } else {
        //  Only Data
        for (unsigned int i=0; i<bins; i++) {
            file << data[i] << " ";
        }
        file << std::endl;
    }
    file.close();
}

bool Histo1d::fromFile(std::string filename) {
<<<<<<< HEAD
    std::ifstream file(filename, std::fstream::in);
    json j;
    try {
        if (!file) {
            throw std::runtime_error("could not open file");
        }
        try {
            j = json::parse(file);
        } catch (json::parse_error &e) {
            throw std::runtime_error(e.what());
        }
    } catch (std::runtime_error &e) {
        hlong->error("Error opening histogram: {}", e.what());
        return false;
    }
    // Check for type
    if (j["Type"].empty()) {
        hlog->error("Tried loading 1d Histogram from file {}, but file has no header: {}", filename);
        return false;
    } else {
        if (j["Type"] == "Histo1d") {
            hlog->error("Tried loading 1d Histogram from file {}, but file has incorrect header: {}", filename, std::string(j["Type"]));
            return false;
        }

        name = j["Name"];
        xAxisTitle = j["x"]["AxisTitle"];
        yAxisTitle = j["y"]["AxisTitle"];
        zAxisTitle = j["z"]["AxisTitle"];

        bins = j["x"]["Bins"];
        xlow = j["x"]["Low"];
        xhigh = j["x"]["High"];

        underflow = j["underflow"];
        overflow = j["overflow"];

        data.resize(bins);
        for (unsigned i=0; i<bins; i++)
            data[i] = j["data"][i];
    }
    file.close();
    return true;
}

void Histo1d::plot(std::string prefix, std::string dir) {
    hlog->info("Plotting: {}", HistogramBase::name);
    // Put raw histo data in tmp file
    std::string tmp_name = std::string(getenv("USER")) + "/tmp_yarr_histo1d_" + prefix;
    this->toFile(tmp_name, "/tmp/", false);
    std::string cmd = "gnuplot | epstopdf -f > " + dir + prefix + "_" + HistogramBase::name + ".pdf";

    // Open gnuplot as file and pipe commands
    FILE *gnu = popen(cmd.c_str(), "w");
    
    fprintf(gnu, "set terminal postscript enhanced color \"Helvetica\" 18 eps\n");
    fprintf(gnu, "unset key\n");
    fprintf(gnu, "set title \"%s\"\n" , HistogramBase::name.c_str());
    fprintf(gnu, "set xlabel \"%s\"\n" , HistogramBase::xAxisTitle.c_str());
    fprintf(gnu, "set ylabel \"%s\"\n" , HistogramBase::yAxisTitle.c_str());
    fprintf(gnu, "set xrange[%f:%f]\n", xlow, xhigh);
    fprintf(gnu, "set yrange[0:*]\n");
    //fprintf(gnu, "set \n");
    fprintf(gnu, "set grid\n");
    fprintf(gnu, "set style line 1 lt 1 lc rgb '#A6CEE3'\n");
    fprintf(gnu, "set style fill solid 0.5\n");
    fprintf(gnu, "set boxwidth %f*0.9 absolute\n", binWidth);
    fprintf(gnu, "plot \"%s\" matrix u ((($1)*(%f))+%f+(%f/2)):3 with boxes\n", ("/tmp/" + tmp_name + "_" + name + ".dat").c_str(), binWidth, xlow, binWidth);
    pclose(gnu);
}
