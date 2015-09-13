/**
 * Licensed to JumpMind Inc under one or more contributor
 * license agreements.  See the NOTICE file distributed
 * with this work for additional information regarding
 * copyright ownership.  JumpMind Inc licenses this file
 * to you under the GNU General Public License, version 3.0 (GPLv3)
 * (the "License"); you may not use this file except in compliance
 * with the License.
 *
 * You should have received a copy of the GNU General Public License,
 * version 3.0 (GPLv3) along with this library; if not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

package org.jumpmind.symmetric4.model;

public class NodeGroupLink extends AbstractObject implements Comparable<NodeGroupLink> {

    private static final long serialVersionUID = 1L;

    private String sourceNodeGroupId;

    private String targetNodeGroupId;

    private String dataEventAction = "W";
    
    private boolean syncConfigEnabled = true; 

    public NodeGroupLink() {   
    }
    
    public NodeGroupLink(String sourceNodeGroupId, String targetNodeGroupId) {
        this.sourceNodeGroupId = sourceNodeGroupId;
        this.targetNodeGroupId = targetNodeGroupId;
    }
    
    public NodeGroupLink(String sourceNodeGroupId, String targetNodeGroupId, String action) {
        this(sourceNodeGroupId, targetNodeGroupId);
        this.dataEventAction = action;
    }
    
    public String getDataEventAction() {
        return dataEventAction;
    }

    public void setDataEventAction(String dataEventAction) {
        this.dataEventAction = dataEventAction;
    }
    
    public void setSyncConfigEnabled(boolean syncConfigEnabled) {
        this.syncConfigEnabled = syncConfigEnabled;
    }
     
    public boolean isSyncConfigEnabled() {
        return syncConfigEnabled;
    }

    public String getSourceNodeGroupId() {
        return sourceNodeGroupId;
    }

    public void setSourceNodeGroupId(String sourceNodeGroupId) {
        this.sourceNodeGroupId = sourceNodeGroupId;
    }

    public String getTargetNodeGroupId() {
        return targetNodeGroupId;
    }

    public void setTargetNodeGroupId(String targetNodeGroupId) {
        this.targetNodeGroupId = targetNodeGroupId;
    }
        
    @Override
    public String toString() {     
        return String.format("%s:%s:%s", sourceNodeGroupId , dataEventAction , targetNodeGroupId);
    }
    
    @Override
    public int compareTo(NodeGroupLink o) {
        return toString().compareTo(o.toString());
    }

    @Override
    public int hashCode() {
        final int prime = 31;
        int result = 1;
        result = prime * result + ((sourceNodeGroupId == null) ? 0 : sourceNodeGroupId.hashCode());
        result = prime * result + ((targetNodeGroupId == null) ? 0 : targetNodeGroupId.hashCode());
        return result;
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj)
            return true;
        if (obj == null)
            return false;
        if (getClass() != obj.getClass())
            return false;
        NodeGroupLink other = (NodeGroupLink) obj;
        if (sourceNodeGroupId == null) {
            if (other.sourceNodeGroupId != null)
                return false;
        } else if (!sourceNodeGroupId.equals(other.sourceNodeGroupId))
            return false;
        if (targetNodeGroupId == null) {
            if (other.targetNodeGroupId != null)
                return false;
        } else if (!targetNodeGroupId.equals(other.targetNodeGroupId))
            return false;
        return true;
    }
    
    
}