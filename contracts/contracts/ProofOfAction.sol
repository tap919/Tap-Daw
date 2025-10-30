// SPDX-License-Identifier: MIT
pragma solidity ^0.8.20;

/**
 * @title ProofOfAction
 * @dev Smart contract for attesting actions with phase tracking (learn/ship/rest)
 */
contract ProofOfAction {
    // Enum representing the different phases
    enum Phase {
        LEARN,
        SHIP,
        REST
    }

    // Event emitted when an action is attested
    event Attested(
        address indexed actor,
        string action,
        Phase phase,
        uint256 timestamp
    );

    // Mapping to track attestations
    mapping(address => uint256) public attestationCount;

    /**
     * @dev Attest an action with a specific phase
     * @param action The action being attested
     * @param phase The current phase (0: LEARN, 1: SHIP, 2: REST)
     */
    function attest(string memory action, Phase phase) external {
        require(bytes(action).length > 0, "Action cannot be empty");
        
        attestationCount[msg.sender]++;
        
        emit Attested(msg.sender, action, phase, block.timestamp);
    }

    /**
     * @dev Get the attestation count for an address
     * @param actor The address to check
     * @return The number of attestations
     */
    function getAttestationCount(address actor) external view returns (uint256) {
        return attestationCount[actor];
    }
}
