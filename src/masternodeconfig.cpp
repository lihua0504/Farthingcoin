
#include "netbase.h"
#include "masternodeconfig.h"
#include "util.h"
#include "chainparams.h"
#include "base58.h"

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

CMasternodeConfig masternodeConfig;

// SPORK_14_MNODES_RELEASE_ENABLED active
void CMasternodeConfig::add(std::string alias, std::string ip, std::string privKey, std::string txHash, std::string outputIndex) {
    CMasternodeEntry cme(alias, ip, privKey, txHash, outputIndex);
    entries.push_back(cme);
}

void CMasternodeConfig::add(std::string alias, std::string ip, std::string privKey, std::string payee) {
    CMasternodeEntry cme(alias, ip, privKey, payee);
    entries.push_back(cme);
}

bool CMasternodeConfig::read(std::string& strErr) {
    int linenumber = 1;
    boost::filesystem::path pathMasternodeConfigFile = GetMasternodeConfigFile();
    boost::filesystem::ifstream streamConfig(pathMasternodeConfigFile);

    if (!streamConfig.good()) {
        FILE* configFile = fopen(pathMasternodeConfigFile.string().c_str(), "a");
        if (configFile != NULL) {
            std::string strHeader = "# Masternode config file\n"
                          "# Format: alias IP:port masternodeprivkey collateral_output_txid collateral_output_index\n"
                          "# Example: mn1 127.0.0.2:9009 8WBA78bgBmkviQiYfPfr6vrkNqViUNCSKTGygkh7UG93vc4ZLcs 9007c19f4257adbc8196127f38b3720c45a6ef3eabfc04a8b7f5836f34546197 1\n";
            fwrite(strHeader.c_str(), std::strlen(strHeader.c_str()), 1, configFile);
            fclose(configFile);
        }
        return true; // Nothing to read, so just return
    }

    for (std::string line; std::getline(streamConfig, line); linenumber++)
    {
        if(line.empty()) continue;

        std::istringstream iss(line);
        std::string comment, alias, ip, privKey, payee, txHash, outputIndex;

        if (iss >> comment) {
            if(comment.at(0) == '#') continue;
            iss.str(line);
            iss.clear();
        }

        if (iss >> alias >> ip >> privKey >> txHash >> outputIndex) {
            // Try to parse masternode configuration in new format - after spork SPORK_14_MNODES_RELEASE_ENABLED
            iss.str(line);
            iss.clear();

            fMasterNodeNewConfig = true;
        } else {
            // Try to parse masternode configuration in old format - before spork SPORK_14_MNODES_RELEASE_ENABLED
            iss.str(line);
            iss.clear();

            if (!(iss >> alias >> ip >> privKey >> payee)) {
                strErr = _("Could not parse masternode.conf") + "\n" + strprintf(_("Line: %d"), linenumber) + "\n\"" + line + "\"";
                streamConfig.close();
                return false;
            }

            iss.str(line);
            iss.clear();
        }

        int port = 0;
        std::string hostname = "";
        SplitHostPort(ip, port, hostname);
        if(port == 0 || hostname == "") {
            strErr = _("Failed to parse host:port string") + "\n"+
                    strprintf(_("Line: %d"), linenumber) + "\n\"" + line + "\"";
            streamConfig.close();
            return false;
        }
        int mainnetDefaultPort = Params(CBaseChainParams::MAIN).GetDefaultPort();
        if(Params().NetworkIDString() == CBaseChainParams::MAIN) {
            if(port != mainnetDefaultPort) {
                strErr = _("Invalid port detected in masternode.conf") + "\n" +
                        strprintf(_("Port: %d"), port) + "\n" +
                        strprintf(_("Line: %d"), linenumber) + "\n\"" + line + "\"" + "\n" +
                        strprintf(_("(must be %d for mainnet)"), mainnetDefaultPort);
                streamConfig.close();
                return false;
            }
        } else if(port == mainnetDefaultPort) {
            strErr = _("Invalid port detected in masternode.conf") + "\n" +
                    strprintf(_("Line: %d"), linenumber) + "\n\"" + line + "\"" + "\n" +
                    strprintf(_("(%d could be used only on mainnet)"), mainnetDefaultPort);
            streamConfig.close();
            return false;
        }

        if (!fMasterNodeNewConfig) {
            CBitcoinAddress address(payee);
            if (!address.IsValid()) {
                strErr = _("Invalid payee address detected in masternode.conf") + "\n" +
                        strprintf(_("Line: %d"), linenumber) + "\n\"" + line + "\"";
                streamConfig.close();
                return false;
            }
        }

        if (fMasterNodeNewConfig) {
            // New masternode config for spork SPORK_14_MNODES_RELEASE_ENABLED
            add(alias, ip, privKey, txHash, outputIndex);
        } else {
            add(alias, ip, privKey, payee);
        }
    }

    streamConfig.close();
    return true;
}
